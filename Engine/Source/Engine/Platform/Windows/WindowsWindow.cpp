// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Logger.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Events/ApplicationEvents.hpp"
#include "Engine/Events/DropEvents.hpp"
#include "Engine/Events/GamepadEvents.hpp"
#include "Engine/Events/KeyboardEvents.hpp"
#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

constexpr int WINDOW_WIDTH = 1600;
constexpr int WINDOW_HEIGHT = 900;

namespace Cobalt
{
    SDL_GLContext gl_context = nullptr;

    auto Window::init(EngineContext& ctx) -> bool {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            Logger::fatal("Engine::Platform", "Failed to initialize SDL3: {}", SDL_GetError());
            return false;
        }
        Logger::trace("Engine::Platform", "SDL3 initialized");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        const auto main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        constexpr auto window_flags =
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        const auto title = ctx.project.get_name();

        m_handle = SDL_CreateWindow(title.c_str(), static_cast<int>(WINDOW_WIDTH * main_scale),
                                    static_cast<int>(WINDOW_HEIGHT * main_scale), window_flags);
        if (m_handle == nullptr) {
            Logger::fatal("Engine::Platform::Window", "Failed to create window: {}", SDL_GetError());
            return false;
        }

        gl_context = SDL_GL_CreateContext(m_handle);
        if (gl_context == nullptr) {
            Logger::fatal("Engine::Platform::Window", "Failed to create OpenGL context: {}", SDL_GetError());
            return false;
        }

        SDL_GL_MakeCurrent(m_handle, gl_context);
        SDL_GL_SetSwapInterval(1);
        SDL_SetWindowPosition(m_handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(m_handle);

        const auto version = gladLoadGL(SDL_GL_GetProcAddress);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (GLAD_GL_ARB_debug_output) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

            glDebugMessageCallbackARB(
                    [](const GLenum source, GLenum type, GLuint id, const GLenum severity, GLsizei length,
                       const GLchar* message, const void* userParam) {
                        if (severity != GL_DEBUG_SEVERITY_HIGH_ARB && severity != GL_DEBUG_SEVERITY_MEDIUM_ARB) {
                            return;
                        }

                        auto src = String{};
                        switch (source) {
                            case GL_DEBUG_SOURCE_API_ARB: src = "API"; break;
                            case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: src = "WindowSys"; break;
                            case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: src = "ShaderCompiler"; break;
                            case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: src = "ThirdParty"; break;
                            case GL_DEBUG_SOURCE_APPLICATION_ARB: src = "Application"; break;
                            case GL_DEBUG_SOURCE_OTHER_ARB: src = "Other"; break;
                            default: src = "NONE"; break;
                        }

                        auto sev = String{};
                        switch (severity) {
                            case GL_DEBUG_SEVERITY_HIGH_ARB: sev = "HIGH"; break;
                            case GL_DEBUG_SEVERITY_MEDIUM_ARB: sev = "MEDIUM"; break;
                            case GL_DEBUG_SEVERITY_LOW_ARB: sev = "LOW"; break;
                            default: sev = "NONE"; break;
                        }

                        Logger::trace("Engine::Platform::OpenGL", "[src={} | sev={}] {}", src, sev, message);
                    },
                    nullptr);
        }

        auto major = GLAD_VERSION_MAJOR(version);
        auto minor = GLAD_VERSION_MINOR(version);

        Logger::trace("Engine::Platform", "OpenGL version: {}", major, minor);
        return true;
    }

    auto Window::poll_events(EngineContext& ctx) const -> void {
        static SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event)) {
            if (m_native_event_hook) {
                m_native_event_hook(&sdl_event);
            }

            switch (sdl_event.type) {
                // Application events
                case SDL_EVENT_QUIT: {
                    ctx.dispatcher.trigger<ApplicationQuitEvent>();
                    ctx.close_requested = true;
                    break;
                }

                // Window events
                case SDL_EVENT_WINDOW_MOVED: {
                    ctx.dispatcher.trigger<WindowMovedEvent>({
                        static_cast<u32>(sdl_event.window.data1),
                        static_cast<u32>(sdl_event.window.data2)
                    });
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: {
                    ctx.dispatcher.trigger<WindowResizedEvent>({
                        static_cast<u32>(sdl_event.window.data1),
                        static_cast<u32>(sdl_event.window.data2)
                    });
                    break;
                }
                case SDL_EVENT_WINDOW_MINIMIZED: {
                    ctx.dispatcher.trigger<WindowMinimizedEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_MAXIMIZED: {
                    ctx.dispatcher.trigger<WindowMaximizedEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_RESTORED: {
                    ctx.dispatcher.trigger<WindowRestoredEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: {
                    ctx.dispatcher.trigger<WindowEnterFullscreenEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: {
                    ctx.dispatcher.trigger<WindowLeaveFullscreenEvent>();
                    break;
                }

                // Keyboard events
                case SDL_EVENT_KEY_DOWN: {
                    ctx.dispatcher.trigger<KeyboardKeyDownEvent>();
                    break;
                }
                case SDL_EVENT_KEY_UP: {
                    ctx.dispatcher.trigger<KeyboardKeyUpEvent>();
                    break;
                }
                case SDL_EVENT_TEXT_INPUT: {
                    ctx.dispatcher.trigger<KeyboardTextInputEvent>();
                    break;
                }

                // Gamepad events
                case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                    ctx.dispatcher.trigger<GamepadAxisMotionEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                    ctx.dispatcher.trigger<GamepadButtonDownEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                    ctx.dispatcher.trigger<GamepadButtonUpEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_ADDED: {
                    ctx.dispatcher.trigger<GamepadAddedEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    ctx.dispatcher.trigger<GamepadRemovedEvent>();
                    break;
                }

                // Drop events
                case SDL_EVENT_DROP_FILE: {
                    ctx.dispatcher.trigger<DropFileEvent>({sdl_event.drop.data});
                    break;
                }
                case SDL_EVENT_DROP_TEXT: {
                    ctx.dispatcher.trigger<DropTextEvent>({sdl_event.drop.data});
                    break;
                }
                case SDL_EVENT_DROP_BEGIN: {
                    ctx.dispatcher.trigger<DropBeginEvent>();
                    break;
                }
                case SDL_EVENT_DROP_COMPLETE: {
                    ctx.dispatcher.trigger<DropCompleteEvent>();
                    break;
                }
                case SDL_EVENT_DROP_POSITION: {
                    ctx.dispatcher.trigger<DropPositionEvent>({
                        static_cast<u32>(sdl_event.drop.x),
                        static_cast<u32>(sdl_event.drop.y)
                    });
                    break;
                }

                default: return;
            }
        }
    }

    auto Window::swap_buffers() const -> void {
        SDL_GL_SwapWindow(m_handle);
    }

    auto Window::get_handle() const -> SDL_Window* {
        return m_handle;
    }

    auto Window::get_gl_context() const -> SDL_GLContextState* {
        return gl_context;
    }

    auto Window::get_size() const -> Vec<2, i32> {
        i32 width, height = 0;
        SDL_GetWindowSize(m_handle, &width, &height);
        return {width, height};
    }

    auto Window::destroy() const -> void {
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(m_handle);
    }

    auto Window::set_native_event_callback(NativeEventCallback callback) -> void {
        m_native_event_hook = std::move(callback);
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Platform/Window.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Events/ApplicationEvents.hpp"
#include "Engine/Events/DropEvents.hpp"
#include "Engine/Events/GamepadEvents.hpp"
#include "Engine/Events/KeyboardEvents.hpp"
#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Events/EventBus.hpp"

#include <SDL3/SDL.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

namespace Cobalt
{
    SDL_GLContext gl_context = nullptr;

    auto Window::Init() -> bool {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            CORE_CRITICAL("Platform: Failed to initialize SDL3: {}", SDL_GetError());
            return false;
        }
        CORE_INFO("Platform: SDL3 initialized.");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        const auto primary_display = SDL_GetPrimaryDisplay();
        constexpr auto window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        const auto title = Project::Get().GetName();

        auto* display_mode = SDL_GetCurrentDisplayMode(primary_display);

        _handle = SDL_CreateWindow(title.c_str(), static_cast<int>(display_mode->w * 0.8),
                                    static_cast<int>(display_mode->h * 0.8), window_flags);
        if (_handle == nullptr) {
            CORE_CRITICAL("Platform: Failed to create window: {}", SDL_GetError());
            return false;
        }

        gl_context = SDL_GL_CreateContext(_handle);
        if (gl_context == nullptr) {
            CORE_CRITICAL("Platform: Failed to create OpenGL context: {}", SDL_GetError());
            return false;
        }

        SDL_GL_MakeCurrent(_handle, gl_context);
        SDL_GL_SetSwapInterval(1);
        SDL_SetWindowPosition(_handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(_handle);

        const auto version = gladLoadGL(SDL_GL_GetProcAddress);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (GLAD_GL_ARB_debug_output) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

            glDebugMessageCallbackARB(
                    [](const GLenum source, GLenum type, GLuint id, const GLenum severity, GLsizei length, const GLchar* message,
                       const void* userParam) {
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

                        CORE_INFO("Platform: OpenGL: [src={} | sev={}] {}", src, sev, message);
                    },
                    nullptr);
        }

        auto major = GLAD_VERSION_MAJOR(version);
        auto minor = GLAD_VERSION_MINOR(version);

        CORE_INFO("Platform: OpenGL version: {}", major, minor);
        return true;
    }

    auto Window::PollEvents(EngineContext& ctx) const -> void {
        static SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event)) {
            if (_native_event_hook) {
                _native_event_hook(&sdl_event);
            }

            switch (sdl_event.type) {
                // Application events
                case SDL_EVENT_QUIT: {
                    EventBus::Trigger<ApplicationQuitEvent>();
                    ctx.close_requested = true;
                    break;
                }

                // Window events
                case SDL_EVENT_WINDOW_MOVED: {
                    EventBus::Trigger<WindowMovedEvent>(
                            {static_cast<u32>(sdl_event.window.data1), static_cast<u32>(sdl_event.window.data2)});
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED: {
                    EventBus::Trigger<WindowResizedEvent>(
                            {static_cast<u32>(sdl_event.window.data1), static_cast<u32>(sdl_event.window.data2)});
                    break;
                }
                case SDL_EVENT_WINDOW_MINIMIZED: {
                    EventBus::Trigger<WindowMinimizedEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_MAXIMIZED: {
                    EventBus::Trigger<WindowMaximizedEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_RESTORED: {
                    EventBus::Trigger<WindowRestoredEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: {
                    EventBus::Trigger<WindowEnterFullscreenEvent>();
                    break;
                }
                case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: {
                    EventBus::Trigger<WindowLeaveFullscreenEvent>();
                    break;
                }

                // Keyboard events
                case SDL_EVENT_KEY_DOWN: {
                    EventBus::Trigger<KeyboardKeyDownEvent>();
                    break;
                }
                case SDL_EVENT_KEY_UP: {
                    EventBus::Trigger<KeyboardKeyUpEvent>();
                    break;
                }
                case SDL_EVENT_TEXT_INPUT: {
                    EventBus::Trigger<KeyboardTextInputEvent>();
                    break;
                }

                // Gamepad events
                case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
                    EventBus::Trigger<GamepadAxisMotionEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
                    EventBus::Trigger<GamepadButtonDownEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_UP: {
                    EventBus::Trigger<GamepadButtonUpEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_ADDED: {
                    EventBus::Trigger<GamepadAddedEvent>();
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    EventBus::Trigger<GamepadRemovedEvent>();
                    break;
                }

                // Drop events
                case SDL_EVENT_DROP_FILE: {
                    EventBus::Trigger<DropFileEvent>({sdl_event.drop.data});
                    break;
                }
                case SDL_EVENT_DROP_TEXT: {
                    EventBus::Trigger<DropTextEvent>({sdl_event.drop.data});
                    break;
                }
                case SDL_EVENT_DROP_BEGIN: {
                    EventBus::Trigger<DropBeginEvent>();
                    break;
                }
                case SDL_EVENT_DROP_COMPLETE: {
                    EventBus::Trigger<DropCompleteEvent>();
                    break;
                }
                case SDL_EVENT_DROP_POSITION: {
                    EventBus::Trigger<DropPositionEvent>({static_cast<u32>(sdl_event.drop.x), static_cast<u32>(sdl_event.drop.y)});
                    break;
                }

                default: return;
            }
        }
    }

    auto Window::SwapBuffers() const -> void {
        SDL_GL_SwapWindow(_handle);
    }

    auto Window::GetHandle() const -> SDL_Window* {
        return _handle;
    }

    auto Window::GetGLContext() const -> SDL_GLContextState* {
        return gl_context;
    }

    auto Window::GetSize() const -> Vec<2, i32> {
        i32 width, height = 0;
        SDL_GetWindowSize(_handle, &width, &height);
        return {width, height};
    }

    auto Window::Get() -> Window& {
        static Window instance;
        return instance;
    }

    auto Window::ShutDown() const -> void {
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(_handle);
    }

    auto Window::SetNativeEventCallback(NativeEventCallback callback) -> void {
        _native_event_hook = std::move(callback);
    }
} // namespace Cobalt

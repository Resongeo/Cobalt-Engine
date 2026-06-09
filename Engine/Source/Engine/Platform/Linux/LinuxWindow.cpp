// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Logger.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

namespace Cobalt
{
    SDL_GLContext gl_context = nullptr;

    auto Window::init(Project& project) -> bool {
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
        const auto title = project.get_name();

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
} // namespace Cobalt

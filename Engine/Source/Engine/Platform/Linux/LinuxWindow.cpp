// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Defines.hpp"

#ifdef PLATFORM_LINUX

#include "Engine/Core/Logger.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

namespace Cobalt
{
    SDL_GLContext gl_context = nullptr;

    auto Window::initialize() -> bool {
        return instance()._create();
    }

    auto Window::_create() -> bool {
        const auto main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        constexpr auto window_flags =
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        m_handle = SDL_CreateWindow("Cobalt Engine", static_cast<int>(WINDOW_WIDTH * main_scale),
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

    auto Window::instance() -> Window& {
        static Window instance;
        return instance;
    }

    auto Window::destroy() -> void {
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(instance().m_handle);
    }
} // namespace Cobalt

#endif

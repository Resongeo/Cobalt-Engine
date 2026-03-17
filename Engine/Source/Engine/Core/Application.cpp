// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Platform/Platform.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

namespace Cobalt::Engine
{
    auto Application::run() -> void {
        _initialize();
        on_begin();
        _main_loop();
        on_end();
        _cleanup();
    }

    auto Application::_initialize() const -> void {
        Platform::initialize();
        Window::initialize();
        Platform::opengl_init();
    }

    auto Application::_main_loop() -> void {
        while (!m_close_requested) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                on_event(&e);

                if (e.type == SDL_EVENT_QUIT) {
                    m_close_requested = true;
                }
            }

            on_update();

            Window::instance().swap_buffers();
        }
    }

    auto Application::_cleanup() const -> void {
        Window::destroy();
    }
}

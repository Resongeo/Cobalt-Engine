// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Platform/Platform.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

namespace Cobalt::Engine
{
    Application::~Application() {
        _cleanup();
    }

    auto Application::run() -> void {
        if (!_initialize()) {
            Logger::fatal("Engine::Core::Application", "Initialization failed. Exiting program.");
            return;
        }

        on_begin();
        _main_loop();
        on_end();
    }

    auto Application::_initialize() const -> bool {
        if (!Platform::sdl3_init()) return false;
        if (!Window::initialize()) return false;
        if (!Platform::opengl_init()) return false;

        return true;
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
} // namespace Cobalt::Engine

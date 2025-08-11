// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Platform/Platform.hpp"

namespace Cobalt::Engine
{
    auto Application::run() -> void {
        initialize();
        on_begin();
        main_loop();
        on_end();
        cleanup();
    }

    auto Application::initialize() -> void {
        Platform::init_glfw();
        p_window.create();
        Platform::init_opengl();
    }

    auto Application::main_loop() -> void {
        while (!p_window.close_requested()) {
            p_window.poll_events();
            on_update();
            p_window.swap_buffers();
        }
    }

    auto Application::cleanup() const -> void {
        p_window.destroy();
    }
}

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Application.hpp"

#include <GLFW/glfw3.h>

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
        m_window.create();
    }

    auto Application::main_loop() -> void {
        while (!m_window.close_requested()) {
            glfwPollEvents();
            on_update();
            m_window.swap_buffers();
        }
    }

    auto Application::cleanup() const -> void {
        m_window.destroy();
    }
}

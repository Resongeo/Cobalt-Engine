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
        m_window.create();
        Platform::init_opengl();
    }

    auto Application::main_loop() -> void {
        while (!m_window.close_requested()) {
            m_window.poll_events();
            on_update();
            m_window.swap_buffers();
        }
    }

    auto Application::cleanup() const -> void {
        m_window.destroy();
    }
}

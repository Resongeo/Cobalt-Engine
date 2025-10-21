// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Platform/Platform.hpp"

namespace Cobalt::Engine
{
    Application* Application::s_instance;

    Application::Application() {
        s_instance = this;
    }

    auto Application::run() -> void {
        _initialize();
        on_begin();
        _main_loop();
        on_end();
        _cleanup();
    }

    auto Application::get_window() -> Window& {
        return s_instance->m_window;
    }

    auto Application::get_scene_manager() -> SceneManager& {
        return s_instance->m_scene_manager;
    }

    auto Application::_initialize() -> void {
        Platform::init_glfw();
        m_window.create();
        Platform::init_opengl();
    }

    auto Application::_main_loop() -> void {
        while (!m_window.close_requested()) {
            m_window.poll_events();
            on_update();
            m_window.swap_buffers();
        }
    }

    auto Application::_cleanup() const -> void {
        m_window.destroy();
    }
}

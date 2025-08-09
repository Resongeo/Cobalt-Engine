// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Defines.hpp"

#ifdef PLATFORM_WINDOWS

#include "Engine/Core/Window.hpp"
#include "Engine/Core/Logger.hpp"

namespace Cobalt::Engine
{
    auto Window::create() -> void {
        if (!glfwInit()) {
            Logger::fatal("Window", "Failed to initialize Glfw");
            std::exit(EXIT_FAILURE);
        }
        Logger::trace("Window", "Glfw initialized");

        // TODO: Create window hints when we are implementing OpenGL
        // TODO: Load window size from config
        m_handle = glfwCreateWindow(
            1280, 720,
            "Cobalt Engine",
            nullptr, nullptr
        );

        if (m_handle == nullptr) {
            Logger::fatal("Window", "Failed to create Glfw window");
            std::exit(EXIT_FAILURE);
        }
        Logger::trace("Window", "Window created with dimensions {}x{}", 1280, 720);

        glfwMakeContextCurrent(m_handle);
    }

    auto Window::close_requested() const -> bool {
        return glfwWindowShouldClose(m_handle);
    }

    auto Window::swap_buffers() const -> void {
        glfwSwapBuffers(m_handle);
    }

    auto Window::destroy() const -> void {
        glfwDestroyWindow(m_handle);
        glfwTerminate();
    }
}

#endif


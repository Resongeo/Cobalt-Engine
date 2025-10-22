// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Defines.hpp"

#ifdef PLATFORM_LINUX

#include "Engine/Core/Window.hpp"
#include "Engine/Core/Logger.hpp"

#include <GLFW/glfw3.h>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

namespace Cobalt::Engine
{
    auto Window::create() -> void {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

        auto* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        const int sys_width = video_mode->width;
        const int sys_height = video_mode->height;
        glfwSetWindowMonitor(m_handle, nullptr,
            sys_width / 2 - WINDOW_WIDTH / 2,
            sys_height / 2 - WINDOW_HEIGHT / 2,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            GLFW_DONT_CARE
        );

        glfwShowWindow(m_handle);
    }

    auto Window::close_requested() const -> bool {
        return glfwWindowShouldClose(m_handle);
    }

    auto Window::poll_events() const -> void {
        glfwPollEvents();
    }

    auto Window::swap_buffers() const -> void {
        glfwSwapBuffers(m_handle);
    }

    auto Window::destroy() const -> void {
        glfwDestroyWindow(m_handle);
        glfwTerminate();
    }

    auto Window::handle() const -> GLFWwindow* {
        return m_handle;
    }

    auto Window::size() const -> Vec<2, i32> {
        i32 width, height = {};
        glfwGetWindowSize(m_handle, &width, &height);

        return {width, height};
    }
}

#endif


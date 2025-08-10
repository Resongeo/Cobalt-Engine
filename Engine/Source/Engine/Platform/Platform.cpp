// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Platform/Platform.hpp"
#include "Engine/Core/Logger.hpp"

#define GLAD_GL_IMPLEMENTATION
#include "Engine/Platform/OpenGL/gl.h"

#include <GLFW/glfw3.h>

namespace Cobalt::Engine
{
    auto Platform::init_glfw() -> void {
        if (!glfwInit()) {
            Logger::fatal("Engine::Platform", "Failed to initialize Glfw");
            std::exit(EXIT_FAILURE);
        }
        Logger::trace("Engine::Platform", "Glfw initialized");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    auto Platform::init_opengl() -> void {
        const auto version = gladLoadGL(glfwGetProcAddress);
        auto major = GLAD_VERSION_MAJOR(version);
        auto minor = GLAD_VERSION_MINOR(version);

        Logger::trace(
            "Engine::Platform",
            "Loaded OpenGL {}.{}",
            major, minor
        );
    }
}
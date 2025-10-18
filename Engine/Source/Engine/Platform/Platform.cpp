// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Platform/Platform.hpp"
#include "Engine/Core/Logger.hpp"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
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
        // TODO: Disable OpenGL debug in Release builds
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }

    auto Platform::init_opengl() -> void {
        const auto version = gladLoadGL(glfwGetProcAddress);

        //glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (GLAD_GL_ARB_debug_output) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

            glDebugMessageCallbackARB(
        [](const GLenum source, GLenum type, GLuint id, const GLenum severity,
           GLsizei length, const GLchar* message, const void* userParam
           ) {
                if (severity != GL_DEBUG_SEVERITY_HIGH_ARB &&
                    severity != GL_DEBUG_SEVERITY_MEDIUM_ARB) {
                    return;
                }

                auto src = String{};
                switch (source) {
                    case GL_DEBUG_SOURCE_API_ARB: src = "API"; break;
                    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: src = "WindowSys"; break;
                    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: src = "ShaderCompiler"; break;
                    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: src = "ThirdParty"; break;
                    case GL_DEBUG_SOURCE_APPLICATION_ARB: src = "Application"; break;
                    case GL_DEBUG_SOURCE_OTHER_ARB: src = "Other"; break;
                    default: src = "NONE"; break;
                }

                auto sev = String{};
                switch (severity) {
                    case GL_DEBUG_SEVERITY_HIGH_ARB: sev = "HIGH"; break;
                    case GL_DEBUG_SEVERITY_MEDIUM_ARB: sev = "MEDIUM"; break;
                    case GL_DEBUG_SEVERITY_LOW_ARB: sev = "LOW"; break;
                    default: sev = "NONE"; break;
                }

                Logger::trace(
                    "OpenGL",
                    "[src={} | sev={}] {}",
                    src,
                    sev,
                    message
                );
            },
        nullptr
            );
        }

        auto major = GLAD_VERSION_MAJOR(version);
        auto minor = GLAD_VERSION_MINOR(version);

        Logger::trace(
            "Engine::Platform",
            "Loaded OpenGL {}.{}",
            major, minor
        );
    }
}
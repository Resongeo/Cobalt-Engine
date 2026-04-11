// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Platform/Platform.hpp"
#include "Engine/Core/Logger.hpp"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <SDL3/SDL.h>

namespace Cobalt::Engine
{
    auto Platform::sdl3_init() -> bool {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            Logger::fatal("Engine::Platform", "Failed to initialize SDL3: {}", SDL_GetError());
            return false;
        }
        Logger::trace("Engine::Platform", "SDL3 initialized");

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        return true;
    }

    auto Platform::opengl_init() -> bool {
        const auto version = gladLoadGL(SDL_GL_GetProcAddress);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (GLAD_GL_ARB_debug_output) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

            glDebugMessageCallbackARB(
                    [](const GLenum source, GLenum type, GLuint id, const GLenum severity, GLsizei length,
                       const GLchar* message, const void* userParam) {
                        if (severity != GL_DEBUG_SEVERITY_HIGH_ARB && severity != GL_DEBUG_SEVERITY_MEDIUM_ARB) {
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

                        Logger::trace("Engine::Platform::OpenGL", "[src={} | sev={}] {}", src, sev, message);
                    },
                    nullptr);
        }

        auto major = GLAD_VERSION_MAJOR(version);
        auto minor = GLAD_VERSION_MINOR(version);

        Logger::trace("Engine::Platform", "OpenGL version: {}", major, minor);

        return true;
    }

    auto Platform::destroy() -> void {
        SDL_Quit();
    }
} // namespace Cobalt::Engine

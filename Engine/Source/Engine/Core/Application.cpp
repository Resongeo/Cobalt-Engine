// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Logger.hpp"

namespace Cobalt::Engine
{
    Application::Application() = default;

    auto Application::run() -> void {
        Logger::trace("test", "message");
        Logger::warn("test", "message");
        Logger::error("test", "message");
        Logger::fatal("test", "message");

        initialize();
        main_loop();
    }

    auto Application::initialize() -> void {
        Logger::trace("Application", "initialize");
    }

    auto Application::main_loop() -> void {
        Logger::trace("Application", "main loop");
    }
}

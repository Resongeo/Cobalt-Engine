// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Application.hpp"

#include <print>

namespace Cobalt::Engine
{
    Application::Application() = default;

    auto Application::run() -> void {
        initialize();
        main_loop();
    }

    auto Application::initialize() -> void {
        std::println("[INFO] Application::initialize");
    }

    auto Application::main_loop() -> void {
        std::println("[INFO] Application::main_loop");
    }
}

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Application.hpp"

namespace Cobalt::Engine
{
    auto Application::run() -> void {
        initialize();
        on_begin();
        main_loop();
        on_end();
    }

    auto Application::initialize() -> void {
    }

    auto Application::main_loop() -> void {
        on_update();
    }
}

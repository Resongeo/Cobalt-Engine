// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

namespace Cobalt::Engine
{
    class Application
    {
    public:
        virtual ~Application() = default;

        auto run() -> void;

        virtual auto on_begin() -> void {}
        virtual auto on_update() -> void {}
        virtual auto on_end() -> void {}

    private:
        auto initialize() -> void;
        auto main_loop() -> void;
    };
}

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

namespace Cobalt::Engine
{
    class Application final
    {
    public:
        explicit Application();

        auto run() -> void;

    private:
        auto initialize() -> void;
        auto main_loop() -> void;
    };
}

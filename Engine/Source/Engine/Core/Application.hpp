// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/CommandLineArgs.hpp"
#include "EngineContext.hpp"

union SDL_Event;

namespace Cobalt
{
    class Application
    {
    public:
        virtual ~Application();

        auto run(const CommandLineArgs& args) -> void;

        virtual auto begin(EngineContext& ctx) -> void {}
        virtual auto update(EngineContext& ctx) -> void {}
        virtual auto end(EngineContext& ctx) -> void {}

    private:
        auto initialize(const CommandLineArgs& args) -> bool;
        auto main_loop() -> void;

    private:
        EngineContext m_ctx = {};
    };
} // namespace Cobalt

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

        auto Run(const CommandLineArgs& args) -> void;

        virtual auto OnBegin(EngineContext& ctx) -> void {}
        virtual auto OnUpdate(EngineContext& ctx) -> void {}
        virtual auto OnShutdown(EngineContext& ctx) -> void {}

    private:
        auto Init(const CommandLineArgs& args) -> bool;
        auto MainLoop() -> void;

    private:
        EngineContext _ctx = {};
    };
} // namespace Cobalt

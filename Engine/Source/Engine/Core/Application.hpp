// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/CommandLineArgs.hpp"
#include "Engine/Events/IEvent.hpp"
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
        virtual auto event(IEvent& event) -> void {}
        // TEMPORARY
        virtual auto on_sdl_event(SDL_Event* event) -> void {}

    private:
        auto initialize(const CommandLineArgs& args) -> bool;
        auto main_loop() -> void;
        auto poll_events() -> void;

    private:
        EngineContext m_ctx = {};
    };
} // namespace Cobalt

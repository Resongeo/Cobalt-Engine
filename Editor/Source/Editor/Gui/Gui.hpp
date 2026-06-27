// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/EngineContext.hpp"
#include "Engine/Platform/Window.hpp"

union SDL_Event;

namespace Cobalt
{
    class Gui final
    {
    public:
        static auto Init(const Window& window) -> void;
        static auto SetupStyle() -> void;
        static auto BeginFrame(const EngineContext& ctx) -> void;
        static auto EndFrame(const EngineContext& ctx) -> void;
        static auto ProcessEvent(const SDL_Event* event) -> void;
    };
} // namespace Cobalt

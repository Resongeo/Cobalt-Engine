// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Platform/Window.hpp"

union SDL_Event;

namespace Cobalt
{
    class Gui final
    {
    public:
        static auto init(const Window& window) -> void;
        static auto begin_frame() -> void;
        static auto process_event(const SDL_Event* event) -> void;
        static auto end_frame() -> void;
    };
} // namespace Cobalt

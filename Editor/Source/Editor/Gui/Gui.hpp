// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Window.hpp"

namespace Cobalt::Editor
{
    class Gui final
    {
    public:
        static auto init(const Engine::Window& window) -> void;
        static auto begin_frame() -> void;
        static auto end_frame() -> void;
    };
}
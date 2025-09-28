// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

namespace Cobalt::Engine
{
    class Renderer final
    {
    public:
        auto init() -> void;
        auto begin_frame() -> void;
        auto end_frame() -> void;
        auto destroy() -> void;
    };
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"

namespace Cobalt
{
    class Time final
    {
    public:
        static auto Init() -> void;
        static auto Update() -> void;
        static auto GetDeltaTime() -> f32;

    private:
        inline static f32 _delta_time = 0.0f;
        inline static u64 _current_time = 0;
        inline static u64 _last_time = 0;
    };
} // namespace Cobalt

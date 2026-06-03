// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/ECS/Entity.hpp"

namespace Cobalt
{
    struct ScriptEntity
    {
        Entity entity;

        auto set_position(f32 x, f32 y) -> void;
        auto get_position_x() -> f32;
    };
}
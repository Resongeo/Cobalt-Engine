// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Math.hpp"
#include "Engine/ECS/Entity.hpp"

namespace Cobalt
{
    struct ScriptEntity
    {
        Entity entity;

        auto set_position(f32 x, f32 y) -> void;
        auto set_position_x(f32 x) -> void;
        auto set_position_y(f32 y) -> void;
        auto get_position() -> Vec2;
        auto get_position_x() -> f32;
        auto get_position_y() -> f32;

        auto set_rotation(f32 degree) -> void;
        auto get_rotation() -> f32;

        auto set_scale(f32 x, f32 y) -> void;
        auto set_scale_x(f32 x) -> void;
        auto set_scale_y(f32 y) -> void;
        auto get_scale() -> Vec2;
        auto get_scale_x() -> f32;
        auto get_scale_y() -> f32;
    };
}
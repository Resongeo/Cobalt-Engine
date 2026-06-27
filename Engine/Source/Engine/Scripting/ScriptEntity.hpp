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

        auto SetPosition(const Vec2& pos) -> void;
        auto SetPositionX(f32 x) -> void;
        auto SetPositionY(f32 y) -> void;
        auto GetPosition() -> Vec2;
        auto GetPositionX() -> f32;
        auto GetPositionY() -> f32;

        auto SetRotation(f32 degree) -> void;
        auto GetRotation() -> f32;

        auto SetScale(const Vec2& scale) -> void;
        auto SetScaleX(f32 x) -> void;
        auto SetScaleY(f32 y) -> void;
        auto GetScale() -> Vec2;
        auto GetScaleX() -> f32;
        auto GetScaleY() -> f32;
    };
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scripting/ScriptEntity.hpp"
#include "Engine/ECS/Components/Minimal.hpp"

namespace Cobalt
{
    auto ScriptEntity::set_position(f32 x, f32 y) -> void {
        if (!entity.is_valid()) {
            return;
        }

        if (entity.has_component<TransformComponent>()) {
            auto& transform = entity.get_component<TransformComponent>();
            transform.position = { x, y };
        }
    }

    auto ScriptEntity::get_position_x() -> f32 {
        if (!entity.is_valid()) {
            return 0.0f;
        }

        return entity.has_component<TransformComponent>() ? entity.get_component<TransformComponent>().position.x : 0.0f;
    }
} // namespace Cobalt

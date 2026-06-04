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

        auto& transform = entity.get_component<TransformComponent>();
        transform.position = { x, y };
    }

    auto ScriptEntity::set_position_x(const f32 x) -> void {
        if (!entity.is_valid()) {
            return;
        }

        auto& transform = entity.get_component<TransformComponent>();
        transform.position.x = x;
    }

    auto ScriptEntity::set_position_y(const f32 y) -> void {
        if (!entity.is_valid()) {
            return;
        }

        auto& transform = entity.get_component<TransformComponent>();
        transform.position.y = y;
    }

    auto ScriptEntity::get_position() -> Vec2 {
        if (!entity.is_valid()) {
            return Vec2{};
        }

        return entity.get_component<TransformComponent>().position;
    }

    auto ScriptEntity::get_position_x() -> f32 {
        if (!entity.is_valid()) {
            return 0.0f;
        }

        return entity.get_component<TransformComponent>().position.x;
    }

    auto ScriptEntity::get_position_y() -> f32 {
        if (!entity.is_valid()) {
            return 0.0f;
        }

        return entity.get_component<TransformComponent>().position.y;
    }

    auto ScriptEntity::set_rotation(const f32 degree) -> void {
        if (!entity.is_valid()) {
            return;
        }

        auto& transform = entity.get_component<TransformComponent>();
        transform.rotation = degree;
    }

    auto ScriptEntity::get_rotation() -> f32 {
        if (!entity.is_valid()) {
            return 0.0f;
        }

        return entity.get_component<TransformComponent>().rotation;
    }

    auto ScriptEntity::set_scale(f32 x, f32 y) -> void {
        if (!entity.is_valid()) {
            return;
        }

        auto& transform = entity.get_component<TransformComponent>();
        transform.scale = { x, y };
    }

    auto ScriptEntity::set_scale_x(const f32 x) -> void {
        if (!entity.is_valid()) {
            return;
        }

        auto& transform = entity.get_component<TransformComponent>();
        transform.scale.x = x;
    }

    auto ScriptEntity::set_scale_y(const f32 y) -> void {
        if (!entity.is_valid()) {
            return;
        }

        auto& transform = entity.get_component<TransformComponent>();
        transform.scale.y = y;
    }

    auto ScriptEntity::get_scale() -> Vec2 {
        if (!entity.is_valid()) {
            return Vec2{};
        }

        return entity.get_component<TransformComponent>().scale;
    }

    auto ScriptEntity::get_scale_x() -> f32 {
        if (!entity.is_valid()) {
            return 0;
        }

        return entity.get_component<TransformComponent>().scale.x;
    }

    auto ScriptEntity::get_scale_y() -> f32 {
        if (!entity.is_valid()) {
            return 0;
        }

        return entity.get_component<TransformComponent>().scale.y;
    }
} // namespace Cobalt

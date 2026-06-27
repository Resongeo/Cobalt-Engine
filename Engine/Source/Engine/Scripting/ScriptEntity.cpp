// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scripting/ScriptEntity.hpp"
#include "Engine/ECS/Components/Minimal.hpp"

namespace Cobalt
{
    auto ScriptEntity::SetPosition(const Vec2& pos) -> void {
        if (!entity.IsValid()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.position = pos;
    }

    auto ScriptEntity::SetPositionX(const f32 x) -> void {
        if (!entity.IsValid()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.position.x = x;
    }

    auto ScriptEntity::SetPositionY(const f32 y) -> void {
        if (!entity.IsValid()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.position.y = y;
    }

    auto ScriptEntity::GetPosition() -> Vec2 {
        if (!entity.IsValid()) {
            return Vec2{};
        }

        return entity.GetComponent<TransformComponent>().position;
    }

    auto ScriptEntity::GetPositionX() -> f32 {
        if (!entity.IsValid()) {
            return 0.0f;
        }

        return entity.GetComponent<TransformComponent>().position.x;
    }

    auto ScriptEntity::GetPositionY() -> f32 {
        if (!entity.IsValid()) {
            return 0.0f;
        }

        return entity.GetComponent<TransformComponent>().position.y;
    }

    auto ScriptEntity::SetRotation(const f32 degree) -> void {
        if (!entity.IsValid()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.rotation = degree;
    }

    auto ScriptEntity::GetRotation() -> f32 {
        if (!entity.IsValid()) {
            return 0.0f;
        }

        return entity.GetComponent<TransformComponent>().rotation;
    }

    auto ScriptEntity::SetScale(const Vec2& scale) -> void {
        if (!entity.IsValid()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.scale = scale;
    }

    auto ScriptEntity::SetScaleX(const f32 x) -> void {
        if (!entity.IsValid()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.scale.x = x;
    }

    auto ScriptEntity::SetScaleY(const f32 y) -> void {
        if (!entity.IsValid()) {
            return;
        }

        auto& transform = entity.GetComponent<TransformComponent>();
        transform.scale.y = y;
    }

    auto ScriptEntity::GetScale() -> Vec2 {
        if (!entity.IsValid()) {
            return Vec2{};
        }

        return entity.GetComponent<TransformComponent>().scale;
    }

    auto ScriptEntity::GetScaleX() -> f32 {
        if (!entity.IsValid()) {
            return 0;
        }

        return entity.GetComponent<TransformComponent>().scale.x;
    }

    auto ScriptEntity::GetScaleY() -> f32 {
        if (!entity.IsValid()) {
            return 0;
        }

        return entity.GetComponent<TransformComponent>().scale.y;
    }
} // namespace Cobalt

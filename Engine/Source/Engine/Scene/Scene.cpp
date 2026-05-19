// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scene/Scene.hpp"
#include "Engine/ECS/Components/Minimal.hpp"

namespace Cobalt
{
    Scene::Scene(const String& name) : m_name(name) {}

    auto Scene::get_name() -> String& {
        return m_name;
    }

    auto Scene::set_name(const String& name) -> void {
        m_name = name;
    }

    auto Scene::get_registry() -> entt::registry& {
        return m_registry;
    }

    auto Scene::create_entity(const String& name) -> Entity {
        const auto id = m_registry.create();
        auto entity = Entity(id, &m_registry);

        entity.add_component<TagComponent>(name);
        entity.add_component<TransformComponent>();

        return entity;
    }

    auto Scene::create_entity(const String& name, const UUID uuid) -> Entity {
        const auto id = m_registry.create();
        auto entity = Entity(id, &m_registry);

        entity.add_component<TagComponent>(name, uuid);
        entity.add_component<TransformComponent>();

        return entity;
    }

    auto Scene::create_empty_entity() -> Entity {
        const auto id = m_registry.create();
        return Entity(id, &m_registry);
    }
} // namespace Cobalt

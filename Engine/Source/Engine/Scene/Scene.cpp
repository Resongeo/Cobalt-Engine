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

    auto Scene::clone() -> Rc<Scene> {
        auto scene = Memory::make_rc<Scene>();
        scene->set_name(m_name);

        const auto view = m_registry.view<entt::entity>();
        for (auto it = view.rbegin(); it != view.rend(); ++it) {
            const auto src_entity = *it;
            const auto dst_entity = scene->m_registry.create();

            if (m_registry.any_of<TagComponent>(src_entity)) {
                const auto src_comp = m_registry.get<TagComponent>(src_entity);
                auto& dst_comp = scene->m_registry.emplace<TagComponent>(dst_entity);

                dst_comp.name = src_comp.name;
                dst_comp.uuid.value = src_comp.uuid.value;
            }

            if (m_registry.any_of<TransformComponent>(src_entity)) {
                const auto src_comp = m_registry.get<TransformComponent>(src_entity);
                auto& dst_comp = scene->m_registry.emplace<TransformComponent>(dst_entity);

                dst_comp.position = src_comp.position;
                dst_comp.scale = src_comp.scale;
                dst_comp.rotation = src_comp.rotation;
            }

            if (m_registry.any_of<SpriteComponent>(src_entity)) {
                const auto src_comp = m_registry.get<SpriteComponent>(src_entity);
                auto& dst_comp = scene->m_registry.emplace<SpriteComponent>(dst_entity);

                dst_comp.texture_id.value = src_comp.texture_id.value;
                dst_comp.tint = src_comp.tint;
            }

            if (m_registry.any_of<ScriptComponent>(src_entity)) {
                const auto src_comp = m_registry.get<ScriptComponent>(src_entity);
                auto& dst_comp = scene->m_registry.emplace<ScriptComponent>(dst_entity);

                dst_comp.script_id.value = src_comp.script_id.value;
            }
        }

        return scene;
    }
} // namespace Cobalt

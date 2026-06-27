// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scene/Scene.hpp"
#include "Engine/ECS/Components/Minimal.hpp"

namespace Cobalt
{
    Scene::Scene(const String& name) : _name(name) {}

    auto Scene::GetName() -> String& {
        return _name;
    }

    auto Scene::SetName(const String& name) -> void {
        this->_name = name;
    }

    auto Scene::GetRegistry() -> entt::registry& {
        return _registry;
    }

    auto Scene::CreateEntity(const String& name) -> Entity {
        const auto id = _registry.create();
        auto entity = Entity(id, &_registry);

        entity.AddComponent<TagComponent>(name);
        entity.AddComponent<TransformComponent>();

        return entity;
    }

    auto Scene::CreateEntity(const String& name, const UUID uuid) -> Entity {
        const auto id = _registry.create();
        auto entity = Entity(id, &_registry);

        entity.AddComponent<TagComponent>(name, uuid);
        entity.AddComponent<TransformComponent>();

        return entity;
    }

    auto Scene::CreateEmptyEntity() -> Entity {
        const auto id = _registry.create();
        return Entity(id, &_registry);
    }

    auto Scene::Clone() -> Rc<Scene> {
        auto scene = Memory::MakeRc<Scene>();
        scene->SetName(_name);

        const auto view = _registry.view<entt::entity>();
        for (auto it = view.rbegin(); it != view.rend(); ++it) {
            const auto src_entity = *it;
            const auto dst_entity = scene->_registry.create();

            if (_registry.any_of<TagComponent>(src_entity)) {
                const auto src_comp = _registry.get<TagComponent>(src_entity);
                auto& dst_comp = scene->_registry.emplace<TagComponent>(dst_entity);

                dst_comp.name = src_comp.name;
                dst_comp.uuid.value = src_comp.uuid.value;
            }

            if (_registry.any_of<TransformComponent>(src_entity)) {
                const auto src_comp = _registry.get<TransformComponent>(src_entity);
                auto& dst_comp = scene->_registry.emplace<TransformComponent>(dst_entity);

                dst_comp.position = src_comp.position;
                dst_comp.scale = src_comp.scale;
                dst_comp.rotation = src_comp.rotation;
            }

            if (_registry.any_of<SpriteComponent>(src_entity)) {
                const auto src_comp = _registry.get<SpriteComponent>(src_entity);
                auto& dst_comp = scene->_registry.emplace<SpriteComponent>(dst_entity);

                dst_comp.texture_id.value = src_comp.texture_id.value;
                dst_comp.tint = src_comp.tint;
            }

            if (_registry.any_of<ScriptComponent>(src_entity)) {
                const auto src_comp = _registry.get<ScriptComponent>(src_entity);
                auto& dst_comp = scene->_registry.emplace<ScriptComponent>(dst_entity);

                dst_comp.script_id.value = src_comp.script_id.value;
            }
        }

        return scene;
    }
} // namespace Cobalt

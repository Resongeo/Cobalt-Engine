// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scene/SceneManager.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"

namespace Cobalt
{
    auto SceneManager::get_scenes() -> Vector<Box<Scene>>& {
        return m_scenes;
    }

    auto SceneManager::get_active_scene() const -> Scene* {
        return m_active_scene;
    }

    auto SceneManager::create_default_scene() -> void {
        const auto& default_scene = m_scenes.emplace_back(Memory::make_box<Scene>("Default"));

        auto entity = default_scene->create_entity("Entity");
        entity.add_component<SpriteComponent>();

        m_active_scene = default_scene.get();
    }

    auto SceneManager::set_state(const SceneState state) -> void {
        m_state = state;
    }

    auto SceneManager::get_state() const -> SceneState {
        return m_state;
    }

    auto SceneManager::update(EngineContext& ctx) -> void {
        if (m_active_scene == nullptr) {
            return;
        }

        auto& registry = m_active_scene->registry();

        switch (m_state) {
            case SceneState::None: {
                for (const auto& system : m_editor_update_systems) {
                    system->update(ctx, registry);
                }
                break;
            }
            case SceneState::Start: {
                for (const auto& system : m_runtime_start_systems) {
                    system->update(ctx, registry);
                }
                m_state = SceneState::Update;
                break;
            }
            case SceneState::Update: {
                for (const auto& system : m_runtime_update_systems) {
                    system->update(ctx, registry);
                }
                break;
            }
        }
    }
} // namespace Cobalt

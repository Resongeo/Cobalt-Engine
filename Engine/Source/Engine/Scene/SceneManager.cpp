// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Scene/SceneManager.hpp"

namespace Cobalt::Engine
{
    auto SceneManager::scenes() -> Vector<Box<Scene>>& {
        return m_scenes;
    }

    auto SceneManager::active_scene() const -> Scene* {
        return m_active_scene;
    }

    auto SceneManager::create_default_scene() -> void {
        const auto& default_scene = m_scenes.emplace_back(
            Memory::make_box<Scene>("Default")
        );
        m_active_scene = default_scene.get();
    }

    auto SceneManager::set_state(const SceneState state) -> void {
        m_state = state;
    }

    auto SceneManager::state() const -> SceneState {
        return m_state;
    }

    auto SceneManager::update() -> void {
        if (m_active_scene == nullptr) {
            return;
        }

        auto& registry = m_active_scene->registry();

        switch (m_state) {
            case SceneState::None: {
                for (const auto& system : m_editor_update_systems) {
                    system->update(registry);
                }
                break;
            }
            case SceneState::Start: {
                for (const auto& system : m_runtime_start_systems) {
                    system->update(registry);
                }
                m_state = SceneState::Update;
                break;
            }
            case SceneState::Update: {
                for (const auto& system : m_runtime_update_systems) {
                    system->update(registry);
                }
                break;
            }
        }
    }
}

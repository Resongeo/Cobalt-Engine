// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"

namespace Cobalt
{
    auto SceneManager::init(EngineContext& ctx) -> void {
        const auto startup_scene_uuid = ctx.project.get_startup_scene_uuid();
        m_active_scene = startup_scene_uuid;
        if (!ctx.asset_manager.get_asset<Scene>(ctx, m_active_scene)) {
            Logger::warn("Engine::SceneManager", "No valid startup scene found in project. Creating default one.");
        }
    }

    auto SceneManager::get_active_scene(EngineContext& ctx) const -> Rc<Scene> {
        return ctx.asset_manager.get_asset<Scene>(ctx, m_active_scene);
    }

    auto SceneManager::get_active_scene_uuid() const -> UUID {
        return m_active_scene;
    }

    auto SceneManager::set_state(const SceneState state) -> void {
        m_state = state;
    }

    auto SceneManager::get_state() const -> SceneState {
        return m_state;
    }

    auto SceneManager::update(EngineContext& ctx) -> void {
        const auto active_scene = get_active_scene(ctx);

        if (active_scene == nullptr) {
            return;
        }

        auto& registry = active_scene->get_registry();

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

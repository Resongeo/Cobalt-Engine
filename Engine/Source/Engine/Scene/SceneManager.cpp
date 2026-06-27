// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"

namespace Cobalt
{
    auto SceneManager::Init(EngineContext& ctx) -> void {
        const auto startup_scene_uuid = ctx.project.StartupSceneUUID();
        if (ctx.asset_manager.IsAssetRegistered(startup_scene_uuid)) {
            _active_scene = ctx.asset_manager.GetAsset<Scene>(ctx, startup_scene_uuid);
            _active_scene_uuid = startup_scene_uuid;
        } else {
            _active_scene_uuid = ctx.asset_manager.CreateInMemoryAsset<Scene>(AssetType::Scene);
            SetActiveScene(ctx, _active_scene_uuid);

            if (_active_scene) {
                _active_scene->SetName("Empty");
            }
        }
    }

    auto SceneManager::GetActiveScene() const -> Rc<Scene> {
        return _active_scene;
    }

    auto SceneManager::GetActiveSceneUUID() const -> UUID {
        return _active_scene_uuid;
    }

    auto SceneManager::SetActiveScene(EngineContext& ctx, const UUID uuid) -> void {
        _active_scene = ctx.asset_manager.GetAsset<Scene>(ctx, uuid);
    }

    auto SceneManager::SetActiveScene(const Rc<Scene>& scene) -> void {
        _active_scene = scene;
    }

    auto SceneManager::SetState(const SceneState state) -> void {
        _state = state;
    }

    auto SceneManager::GetState() const -> SceneState {
        return _state;
    }

    auto SceneManager::Update(EngineContext& ctx) -> void {
        const auto active_scene = GetActiveScene();

        if (active_scene == nullptr) {
            return;
        }

        auto& registry = active_scene->GetRegistry();

        switch (_state) {
            case SceneState::None: {
                for (const auto& system : _editor_update_systems) {
                    system->Update(ctx, registry);
                }
                break;
            }
            case SceneState::Start: {
                for (const auto& system : _runtime_start_systems) {
                    system->Update(ctx, registry);
                }
                _state = SceneState::Update;
                break;
            }
            case SceneState::Update: {
                for (const auto& system : _runtime_update_systems) {
                    system->Update(ctx, registry);
                }
                break;
            }
        }
    }
} // namespace Cobalt

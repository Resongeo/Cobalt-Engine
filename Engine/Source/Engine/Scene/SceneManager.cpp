// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"

#include <optick.h>

namespace Cobalt
{
    auto SceneManager::Init() -> Result<bool, CoreInitError> {
        const auto startup_scene_uuid = Project::Get().GetStartupSceneUUID();

        if (AssetManager::Get().IsAssetRegistered(startup_scene_uuid)) {
            _active_scene = AssetManager::Get().GetAsset<Scene>(startup_scene_uuid);
            _active_scene_uuid = startup_scene_uuid;
        } else {
            _active_scene_uuid = AssetManager::Get().CreateInMemoryAsset<Scene>(AssetType::Scene);
            SetActiveScene(_active_scene_uuid);

            if (_active_scene) {
                _active_scene->SetName("Empty");
            }
        }

        return true;
    }

    auto SceneManager::Shutdown() -> void {
        if (_active_scene) {
            _active_scene->Destroy();
            _active_scene.reset();
        }

        _editor_update_systems.clear();
        _runtime_start_systems.clear();
        _runtime_update_systems.clear();
    }

    auto SceneManager::GetActiveScene() const -> Rc<Scene> {
        return _active_scene;
    }

    auto SceneManager::GetActiveSceneUUID() const -> UUID {
        return _active_scene_uuid;
    }

    auto SceneManager::SetActiveScene(const UUID uuid) -> void {
        _active_scene = AssetManager::Get().GetAsset<Scene>(uuid);
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

    auto SceneManager::Update() -> void {
        OPTICK_EVENT();

        const auto active_scene = GetActiveScene();

        if (active_scene == nullptr) {
            return;
        }

        auto& registry = active_scene->GetRegistry();

        switch (_state) {
            case SceneState::None: {
                for (const auto& system : _editor_update_systems) {
                    system->Update(registry);
                }
                break;
            }
            case SceneState::Start: {
                for (const auto& system : _runtime_start_systems) {
                    system->Update(registry);
                }
                _state = SceneState::Update;
                break;
            }
            case SceneState::Update: {
                for (const auto& system : _runtime_update_systems) {
                    system->Update(registry);
                }
                break;
            }
        }
    }

    auto SceneManager::Get() -> SceneManager& {
        static SceneManager instance;
        return instance;
    }
} // namespace Cobalt

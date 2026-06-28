// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Fwd.hpp"
#include "Engine/ECS/Systems/ISystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Scene/SceneState.hpp"

namespace Cobalt
{
    class SceneManager final
    {
    public:
        ~SceneManager() = default;

        auto Init() -> void;

        auto GetActiveScene() const -> Rc<Scene>;
        auto GetActiveSceneUUID() const -> UUID;
        auto SetActiveScene(UUID uuid) -> void;
        auto SetActiveScene(const Rc<Scene>& scene) -> void;
        auto GetState() const -> SceneState;
        auto SetState(SceneState state) -> void;
        auto Update() -> void;

        template <typename T, typename... Args>
        auto AddSystem(const Schedule schedule, Args&&... args) -> void {
            switch (schedule) {
                case Schedule::RuntimeStart: {
                    _runtime_start_systems.emplace_back(Memory::MakeBox<T>(std::forward<Args>(args)...));
                    break;
                }
                case Schedule::EditorUpdate: {
                    _editor_update_systems.emplace_back(Memory::MakeBox<T>(std::forward<Args>(args)...));
                    break;
                }
                case Schedule::RuntimeUpdate: {
                    _runtime_update_systems.emplace_back(Memory::MakeBox<T>(std::forward<Args>(args)...));
                    break;
                }
            }
        }

        static auto Get() -> SceneManager&;

    private:
        Rc<Scene> _active_scene = nullptr;
        UUID _active_scene_uuid;
        SceneState _state = SceneState::None;

        Vector<Box<ISystem>> _runtime_start_systems = {};
        Vector<Box<ISystem>> _editor_update_systems = {};
        Vector<Box<ISystem>> _runtime_update_systems = {};
    };
} // namespace Cobalt

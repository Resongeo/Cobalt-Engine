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

        auto init(EngineContext& ctx) -> void;

        auto get_active_scene(EngineContext& ctx) const -> Rc<Scene>;
        auto get_active_scene_uuid() const -> UUID;
        auto set_active_scene(EngineContext& ctx, UUID uuid) -> void;
        auto get_state() const -> SceneState;
        auto set_state(SceneState state) -> void;
        auto update(EngineContext& ctx) -> void;

        template <typename T, typename... Args>
        auto add_system(const Schedule schedule, Args&&... args) -> void {
            switch (schedule) {
                case Schedule::RuntimeStart: {
                    m_runtime_start_systems.emplace_back(Memory::make_box<T>(std::forward<Args>(args)...));
                    break;
                }
                case Schedule::EditorUpdate: {
                    m_editor_update_systems.emplace_back(Memory::make_box<T>(std::forward<Args>(args)...));
                    break;
                }
                case Schedule::RuntimeUpdate: {
                    m_runtime_update_systems.emplace_back(Memory::make_box<T>(std::forward<Args>(args)...));
                    break;
                }
            }
        }

    private:
        Rc<Scene> m_active_scene = nullptr;
        UUID m_active_scene_uuid;
        SceneState m_state = SceneState::None;

        Vector<Box<ISystem>> m_runtime_start_systems = {};
        Vector<Box<ISystem>> m_editor_update_systems = {};
        Vector<Box<ISystem>> m_runtime_update_systems = {};
    };
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Engine/Scene/SceneState.hpp"
#include "Engine/ECS/Systems/ISystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"

namespace Cobalt::Engine
{
    class SceneManager final
    {
    public:
        ~SceneManager() = default;

        auto get_scenes() -> Vector<Box<Scene>>&;
        auto get_active_scene() const -> Scene*;
        auto get_state() const -> SceneState;
        auto set_state(SceneState state) -> void;
        auto create_default_scene() -> void;
        auto update() -> void;

        static auto instance() -> SceneManager&;

        template<typename T, typename... Args>
        auto add_system(const Schedule schedule, Args&&... args) -> void {
            switch (schedule) {
                case Schedule::RuntimeStart: {
                    m_runtime_start_systems.emplace_back(
                        Memory::make_box<T>(std::forward<Args>(args)...)
                    );
                    break;
                }
                case Schedule::EditorUpdate: {
                    m_editor_update_systems.emplace_back(
                        Memory::make_box<T>(std::forward<Args>(args)...)
                    );
                    break;
                }
                case Schedule::RuntimeUpdate: {
                    m_runtime_update_systems.emplace_back(
                        Memory::make_box<T>(std::forward<Args>(args)...)
                    );
                    break;
                }
            }
        }

    private:
        Vector<Box<Scene>> m_scenes = {};
        Scene* m_active_scene = nullptr;
        SceneState m_state = SceneState::None;

        Vector<Box<ISystem>> m_runtime_start_systems = {};
        Vector<Box<ISystem>> m_editor_update_systems = {};
        Vector<Box<ISystem>> m_runtime_update_systems = {};
    };
}
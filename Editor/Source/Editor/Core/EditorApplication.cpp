// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Core/Project.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/TagComponent.hpp"
#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/Scene/Scene.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    EditorApplication::EditorApplication(const i32 argc, char* argv[])
        : m_argc(argc), m_argv(argv) {
    }

    auto EditorApplication::on_begin() -> void {
        Project::init();
        Project::parse(m_argc, m_argv);
        Gui::init(Application::get_window());

        Application::get_scene_manager().create_default_scene();

        m_renderer.init(3, Project::get_editor_assets_path());

        // TODO: Give it a Framebuffer later
        Application::get_scene_manager().add_system<Engine::EditorRenderSystem>(
            Engine::Schedule::EditorUpdate,
            &m_renderer, &m_camera
        );
    }

    auto EditorApplication::on_update() -> void {
        auto& scene_manager = Application::get_scene_manager();
        scene_manager.update();

        Gui::begin_frame();
        {
            ImGui::Begin("Debug");
            {
                _draw_project_window();
                _draw_scenes_window();
                _draw_entities_window();
                _draw_components_window();

                if (ImGui::CollapsingHeader("Camera")) {
                    ImGui::DragFloat2("Position", &m_camera.position[0], 0.1);
                    ImGui::SliderFloat("Size", &m_camera.size, 0.1, 20);
                    ImGui::DragFloat("Rotation", &m_camera.rotation, 0.1);
                    ImGui::ColorEdit3("Clear Color", &m_camera.clear_color.r);
                }

                switch (scene_manager.state()) {
                    case Engine::SceneState::None: {
                        if (ImGui::Button("Start Scene")) {
                            scene_manager.set_state(Engine::SceneState::Start);
                        }
                        break;
                    }
                    case Engine::SceneState::Start:
                    case Engine::SceneState::Update: {
                        if (ImGui::Button("Stop Scene")) {
                            scene_manager.set_state(Engine::SceneState::None);
                        }
                        break;
                    }
                }
            }
            ImGui::End();
        }
        Gui::end_frame();
    }

    auto EditorApplication::on_end() -> void {
    }

    auto EditorApplication::_draw_project_window() const -> void {
        if (ImGui::CollapsingHeader("Project")) {
            ImGui::Text("Name: %s", Project::get_name().c_str());
            ImGui::Text("Version: %s", Project::get_version().c_str());
            ImGui::Text("Version: %s", Project::get_version().c_str());
            ImGui::Separator();
            ImGui::Text("Asset paths");
            ImGui::Text("Editor: %s", Project::get_editor_assets_path().c_str());
            ImGui::Text("Project: %s", Project::get_project_assets_path().c_str());
        }
    }

    auto EditorApplication::_draw_scenes_window() const -> void {
        auto& scene_manager = Application::get_scene_manager();

        if (ImGui::CollapsingHeader("Scenes")) {
            if (auto* scene = scene_manager.active_scene(); scene != nullptr) {
                ImGui::Text("Scene: %s", scene->name().c_str());

                const auto& scenes = scene_manager.scenes();
                for (auto i = 0; i < scenes.size(); i++) {
                    ImGui::Text("%d %s", i, scenes[i]->name().c_str());
                }
            }
        }
    }

    auto EditorApplication::_draw_entities_window() -> void {
        const auto& scene_manager = Application::get_scene_manager();
        auto* scene = scene_manager.active_scene();
        if (scene == nullptr) {
            return;
        }

        if (ImGui::CollapsingHeader("Entities")) {
            if (ImGui::Button("Create")) {
                for (auto i = 0; i < 10; i++) {
                    const auto entity = scene->registry().create();
                    auto& [name, uuid] = scene->registry().emplace<Engine::TagComponent>(entity);
                    name = std::format("Entity {}", static_cast<u32>(entity) + 1);
                    uuid = UUID::generate();
                    scene->registry().emplace<Engine::SpriteComponent>(entity);
                    scene->registry().emplace<Engine::TransformComponent>(entity);
                }
            }

            if (m_state.selected_entity != entt::null) {
                ImGui::SameLine();
                if (ImGui::Button("Deselect")) {
                    m_state.selected_entity = entt::null;
                }
            }

            for (const auto entity : scene->registry().view<entt::entity>()) {
                auto label = std::format("Entity id: {}", static_cast<u32>(entity));

                if (m_state.selected_entity == entity) {
                    ImGui::PushStyleColor(ImGuiCol_Button, {0.8, 0.3, 0.3, 1.0});
                    if (ImGui::Button(label.c_str())) {
                        m_state.selected_entity = entity;
                    }
                    ImGui::PopStyleColor();
                } else {
                    if (ImGui::Button(label.c_str())) {
                        m_state.selected_entity = entity;
                    }
                }
            }
        }
    }

    auto EditorApplication::_draw_components_window() const -> void {
        const auto& scene_manager = Application::get_scene_manager();
        if (m_state.selected_entity == entt::null) {
            return;
        }

        auto* scene = scene_manager.active_scene();

        if (scene == nullptr) {
            return;
        }

        ImGui::Begin("Components");
        {
            if (scene->registry().any_of<Engine::TagComponent>(m_state.selected_entity)) {
                auto& [name, uuid] = scene->registry().get<Engine::TagComponent>(m_state.selected_entity);
                ImGui::Text("Name: %s", name.c_str());
                ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
            }

            if (scene->registry().any_of<Engine::TransformComponent>(m_state.selected_entity)) {
                auto& [pos, scale] = scene->registry().get<Engine::TransformComponent>(m_state.selected_entity);
                ImGui::DragFloat2("Position", &pos[0], 0.1f);
                ImGui::DragFloat2("Scale", &scale[0], 0.1f);
            }

            if (scene->registry().any_of<Engine::SpriteComponent>(m_state.selected_entity)) {
                auto& [tint] = scene->registry().get<Engine::SpriteComponent>(m_state.selected_entity);
                ImGui::ColorEdit4("Tint", &tint.r);
            }
        }
        ImGui::End();
    }
}

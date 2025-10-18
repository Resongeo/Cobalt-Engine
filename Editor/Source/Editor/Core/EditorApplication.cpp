// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/ECS/Components/TagComponent.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    EditorApplication::EditorApplication(const i32 argc, char* argv[])
        : m_project(argc, argv) {
    }

    auto EditorApplication::on_begin() -> void {
        m_project.parse();
        if (m_project.startup_scene().is_valid()) {
            // TODO: Load scene from UUID
        } else {
            p_scene_manager.create_default_scene();
        }

        Gui::init(p_window);

        const auto shader = Memory::make_rc<Engine::Shader>();
        shader->create(
            "Assets\\Shaders\\DefaultQuad.vert",
            "Assets\\Shaders\\DefaultQuad.frag"
        );

        m_renderer.init(100, shader);
    }

    auto EditorApplication::on_update() -> void {
        m_renderer.begin_frame();
        m_renderer.submit_quad(
            {0, 0, 0},
            {1, 1},
            {0.8, 0.3, 0.3, 1.0});
        m_renderer.end_frame();

        Gui::begin_frame();
        {
            /*
            ImGui::Begin("Debug");
            {
                _draw_project_window();
                _draw_scenes_window();
                _draw_entities_window();
                _draw_components_window();
            }
            ImGui::End();
            */
        }
        Gui::end_frame();
    }

    auto EditorApplication::on_end() -> void {
    }

    auto EditorApplication::_draw_project_window() -> void {
        if (ImGui::CollapsingHeader("Project")) {
            ImGui::Text("Name: %s", m_project.name().c_str());
            ImGui::Text("Version: %s", m_project.version().c_str());
        }
    }

    auto EditorApplication::_draw_scenes_window() -> void {
        if (ImGui::CollapsingHeader("Scenes")) {
            if (auto* scene = p_scene_manager.active_scene(); scene != nullptr) {
                ImGui::Text("Scene: %s", scene->name().c_str());

                const auto& scenes = p_scene_manager.scenes();
                for (auto i = 0; i < scenes.size(); i++) {
                    ImGui::Text("%d %s", i, scenes[i]->name().c_str());
                }
            }
        }
    }

    auto EditorApplication::_draw_entities_window() -> void {
        auto* scene = p_scene_manager.active_scene();
        if (scene == nullptr) {
            return;
        }

        if (ImGui::CollapsingHeader("Entities")) {
            if (ImGui::Button("Create")) {
                const auto entity = scene->registry().create();
                auto& [name, uuid] = scene->registry().emplace<Engine::TagComponent>(entity);
                name = std::format("Entity {}", static_cast<u32>(entity) + 1);
                uuid = UUID::generate();
            }

            if (m_selected_entity != entt::null) {
                ImGui::SameLine();
                if (ImGui::Button("Deselect")) {
                    m_selected_entity = entt::null;
                }
            }

            for (const auto entity : scene->registry().view<entt::entity>()) {
                auto label = std::format("Entity id: {}", static_cast<u32>(entity));

                if (m_selected_entity == entity) {
                    ImGui::PushStyleColor(ImGuiCol_Button, {0.8, 0.3, 0.3, 1.0});
                    if (ImGui::Button(label.c_str())) {
                        m_selected_entity = entity;
                    }
                    ImGui::PopStyleColor();
                } else {
                    if (ImGui::Button(label.c_str())) {
                        m_selected_entity = entity;
                    }
                }
            }
        }
    }

    auto EditorApplication::_draw_components_window() const -> void {
        if (m_selected_entity == entt::null) {
            return;
        }

        auto* scene = p_scene_manager.active_scene();

        if (scene == nullptr) {
            return;
        }

        ImGui::Begin("Components");
        {
            if (scene->registry().any_of<Engine::TagComponent>(m_selected_entity)) {
                auto& [name, uuid] = scene->registry().get<Engine::TagComponent>(m_selected_entity);
                ImGui::Text("Name: %s", name.c_str());
                ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
            }
        }
        ImGui::End();
    }
}

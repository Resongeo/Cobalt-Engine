// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Entity.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    auto SceneHierarchyPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Scene Hierarchy");
        {
            if (ImGui::Button("Create")) {
                auto entity = state.active_scene->create_entity("Entity");
                entity.add_component<Engine::SpriteComponent>();
            }

            if (state.selected_entity != entt::null) {
                ImGui::SameLine();
                if (ImGui::Button("Deselect")) {
                    state.selected_entity = entt::null;
                }
            }

            for (const auto entity : state.active_scene->registry().view<entt::entity>()) {
                auto name = state.active_scene->registry().get<Engine::TagComponent>(entity).name;

                if (state.selected_entity == entity) {
                    ImGui::PushStyleColor(ImGuiCol_Button, {0.8, 0.3, 0.3, 1.0});
                    if (ImGui::Button(name.c_str())) {
                        state.selected_entity = entity;
                    }
                    ImGui::PopStyleColor();
                } else {
                    if (ImGui::Button(name.c_str())) {
                        state.selected_entity = entity;
                    }
                }
            }
        }
        ImGui::End();
    }
} // namespace Cobalt::Editor

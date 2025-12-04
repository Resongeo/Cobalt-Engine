// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"
#include "Engine/ECS/Components/TagComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

namespace Cobalt::Editor
{
    auto SceneHierarchyPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Scene Hierarchy");
        {
            if (ImGui::Button("Create")) {
                const auto entity = state.active_scene->registry().create();
                auto& [name, uuid] = state.active_scene->registry().emplace<Engine::TagComponent>(entity);
                name = std::format("Entity {}", static_cast<u32>(entity) + 1);
                uuid = UUID::generate();
                state.active_scene->registry().emplace<Engine::SpriteComponent>(entity);
                state.active_scene->registry().emplace<Engine::TransformComponent>(entity);
            }

            if (state.selected_entity != entt::null) {
                ImGui::SameLine();
                if (ImGui::Button("Deselect")) {
                    state.selected_entity = entt::null;
                }
            }

            for (const auto entity : state.active_scene->registry().view<entt::entity>()) {
                auto label = std::format("Entity id: {}", static_cast<u32>(entity));

                if (state.selected_entity == entity) {
                    ImGui::PushStyleColor(ImGuiCol_Button, {0.8, 0.3, 0.3, 1.0});
                    if (ImGui::Button(label.c_str())) {
                        state.selected_entity = entity;
                    }
                    ImGui::PopStyleColor();
                } else {
                    if (ImGui::Button(label.c_str())) {
                        state.selected_entity = entity;
                    }
                }
            }
        }
        ImGui::End();
    }
}

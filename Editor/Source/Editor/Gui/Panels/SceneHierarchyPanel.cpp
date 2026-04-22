// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Entity.hpp"

#include <imgui.h>

namespace Cobalt
{
    auto SceneHierarchyPanel::draw(EngineContext& ctx, EditorState& state) -> void {
        Widgets::begin("Scene Hierarchy");
        {
            if (Widgets::button("Create")) {
                auto entity = state.active_scene->create_entity("Entity");
                entity.add_component<SpriteComponent>();
            }

            if (state.selected_entity != entt::null) {
                ImGui::SameLine();
                if (Widgets::button("Deselect")) {
                    state.selected_entity = entt::null;
                }
            }

            for (const auto entity : state.active_scene->get_registry().view<entt::entity>()) {
                auto [name, id] = state.active_scene->get_registry().get<TagComponent>(entity);

                ImGui::PushID(id.value);
                if (state.selected_entity == entity) {
                    ImGui::PushStyleColor(ImGuiCol_Button, {0.8, 0.3, 0.3, 1.0});
                    if (Widgets::button(name.c_str())) {
                        state.selected_entity = entity;
                    }
                    ImGui::PopStyleColor();
                } else {
                    if (Widgets::button(name.c_str())) {
                        state.selected_entity = entity;
                    }
                }
                ImGui::PopID();
            }
        }
        Widgets::end();
    }
} // namespace Cobalt

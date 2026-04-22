// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Entity.hpp"

#include <imgui.h>

namespace Cobalt
{
    auto EntityComponentsPanel::draw(EngineContext& ctx, EditorState& state) -> void {
        Widgets::begin("Components", {8, 8});
        {
            if (state.selected_entity != entt::null) {
                auto entity = Entity(state.selected_entity, &state.active_scene->get_registry());

                if (entity.has_component<TagComponent>()) {
                    auto& [name, uuid] = entity.get_component<TagComponent>();
                    ImGui::Text("Name: %s", name.c_str());
                    ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
                }

                if (entity.has_component<TransformComponent>()) {
                    auto& [position, scale, rotation] = entity.get_component<TransformComponent>();
                    ImGui::DragFloat2("Position", &position[0], 0.1f);
                    ImGui::DragFloat2("Scale", &scale[0], 0.1f);
                    ImGui::DragFloat("Rotation", &rotation, 0.1f);
                }

                if (entity.has_component<SpriteComponent>()) {
                    auto& [tint, uuid] = entity.get_component<SpriteComponent>();
                    ImGui::ColorEdit4("Tint", &tint.r);
                    ImGui::InputScalar("UUID", ImGuiDataType_U64, (void*)&uuid.value);
                }
            }
        }
        Widgets::end();
    }
} // namespace Cobalt

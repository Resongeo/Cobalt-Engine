// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Entity.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    auto EntityComponentsPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Components");
        {
            if (state.selected_entity != entt::null) {
                auto entity = Engine::Entity(state.selected_entity, &state.active_scene->registry());

                if (entity.has_component<Engine::TagComponent>()) {
                    auto& [name, uuid] = entity.get_component<Engine::TagComponent>();
                    ImGui::Text("Name: %s", name.c_str());
                    ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
                }

                if (entity.has_component<Engine::TransformComponent>()) {
                    auto& [position, scale, rotation] = entity.get_component<Engine::TransformComponent>();
                    ImGui::DragFloat2("Position", &position[0], 0.1f);
                    ImGui::DragFloat2("Scale", &scale[0], 0.1f);
                    ImGui::DragFloat("Rotation", &rotation, 0.1f);
                }

                if (entity.has_component<Engine::SpriteComponent>()) {
                    auto& [tint] = entity.get_component<Engine::SpriteComponent>();
                    ImGui::ColorEdit4("Tint", &tint.r);
                }
            }
        }
        ImGui::End();
    }
} // namespace Cobalt::Editor

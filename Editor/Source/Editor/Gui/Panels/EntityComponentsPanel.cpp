// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Engine/ECS/Components/TagComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    auto EntityComponentsPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Components");
        {
            if (state.selected_entity != entt::null) {
                auto& ecs_registry = state.active_scene->registry();

                if (ecs_registry.any_of<Engine::TagComponent>(state.selected_entity)) {
                    auto& [name, uuid] = ecs_registry.get<Engine::TagComponent>(state.selected_entity);
                    ImGui::Text("Name: %s", name.c_str());
                    ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
                }
                
                if (ecs_registry.any_of<Engine::TransformComponent>(state.selected_entity)) {
                    auto& [position, scale] = ecs_registry.get<Engine::TransformComponent>(state.selected_entity);
                    ImGui::DragFloat2("Position", &position[0], 0.1f);
                    ImGui::DragFloat2("Scale", &scale[0], 0.1f);
                }
                
                if (ecs_registry.any_of<Engine::SpriteComponent>(state.selected_entity)) {
                    auto& [tint] = ecs_registry.get<Engine::SpriteComponent>(state.selected_entity);
                    ImGui::ColorEdit4("Tint", &tint.r);
                }
            }
        }
        ImGui::End();
    }
}

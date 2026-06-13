// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Entity.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Cobalt
{
    auto EntityComponentsPanel::draw(EngineContext& ctx, EditorState& state) -> void {
        Widgets::begin("Components", {8, 8});
        {
            if (state.selected_entity != entt::null) {
                auto entity = Entity(state.selected_entity, &state.active_scene->get_registry());

                ImGui::PushFont(Fonts::icon);
                if (Widgets::button(ICON_PLUS, Variant::Default, {0, 0}, true)) {
                    ImGui::OpenPopup("Add Component Popup Menu");
                }
                ImGui::PopFont();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});
                if (ImGui::BeginPopupContextItem("Add Component Popup Menu")) {
                    ImGui::SeparatorText("Add Component");

                    if (!entity.has_component<SpriteComponent>()) {
                        if (Widgets::button("Sprite Component")) {
                            entity.add_component<SpriteComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (!entity.has_component<ScriptComponent>()) {
                        if (Widgets::button("Script Component")) {
                            entity.add_component<ScriptComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar(2);

                if (entity.has_component<TagComponent>()) {
                    if (Widgets::collapsing_header("Tag", Colors::tag)) {
                        auto& [name, uuid] = entity.get_component<TagComponent>();
                        ImGui::InputText("Name", &name);

                        ImGui::PushStyleColor(ImGuiCol_Text, IMVEC4(Colors::text_muted));
                        ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
                        ImGui::PopStyleColor();
                    }
                }

                if (entity.has_component<TransformComponent>()) {
                    if (Widgets::collapsing_header("Transform", Colors::transform)) {
                        auto& [position, scale, rotation] = entity.get_component<TransformComponent>();
                        ImGui::DragFloat2("Position", &position[0], 0.1f);
                        ImGui::DragFloat2("Scale", &scale[0], 0.1f);
                        ImGui::DragFloat("Rotation", &rotation, 0.1f);
                    }
                }

                if (entity.has_component<SpriteComponent>()) {
                    if (Widgets::collapsing_header("Sprite", Colors::sprite)) {
                        auto& [tint, uuid] = entity.get_component<SpriteComponent>();
                        ImGui::ColorEdit4("Tint", &tint.r);
                        ImGui::InputScalar("Texture UUID", ImGuiDataType_U64, (void*)&uuid.value);

                        if (ImGui::BeginDragDropTarget()) {
                            if (const auto* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_AND_DROP")) {
                                const u64 uid = *static_cast<u64*>(payload->Data);
                                uuid.value = uid;
                            }
                            ImGui::EndDragDropTarget();
                        }
                    }
                }

                if (entity.has_component<ScriptComponent>()) {
                    if (Widgets::collapsing_header("Script", Colors::script)) {
                        auto& [uuid, _] = entity.get_component<ScriptComponent>();
                        ImGui::InputScalar("Script UUID", ImGuiDataType_U64, (void*)&uuid.value);

                        if (ImGui::BeginDragDropTarget()) {
                            if (const auto* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_AND_DROP")) {
                                const u64 uid = *static_cast<u64*>(payload->Data);
                                uuid.value = uid;
                            }
                            ImGui::EndDragDropTarget();
                        }
                    }
                }
            }
        }
        Widgets::end();
    }
} // namespace Cobalt

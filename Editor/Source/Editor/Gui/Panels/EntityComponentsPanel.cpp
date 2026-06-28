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
    auto EntityComponentsPanel::Draw(EditorState& state) -> void {
        Widgets::Begin("Components", {8, 8});
        {
            if (state.selected_entity != entt::null) {
                auto entity = Entity(state.selected_entity, &state.active_scene->GetRegistry());

                ImGui::PushFont(Fonts::icon);
                if (Widgets::Button(ICON_PLUS, Variant::Default, {0, 0}, true)) {
                    ImGui::OpenPopup("Add Component Popup Menu");
                }
                ImGui::PopFont();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});
                if (ImGui::BeginPopupContextItem("Add Component Popup Menu")) {
                    ImGui::SeparatorText("Add Component");

                    if (!entity.HasComponent<SpriteComponent>()) {
                        if (Widgets::Button("Sprite Component")) {
                            entity.AddComponent<SpriteComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (!entity.HasComponent<ScriptComponent>()) {
                        if (Widgets::Button("Script Component")) {
                            entity.AddComponent<ScriptComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                }
                ImGui::PopStyleVar(2);

                if (entity.HasComponent<TagComponent>()) {
                    if (Widgets::CollapsingHeader("Tag", Colors::tag)) {
                        auto& [name, uuid] = entity.GetComponent<TagComponent>();
                        ImGui::InputText("Name", &name);

                        ImGui::PushStyleColor(ImGuiCol_Text, IMVEC4(Colors::text_muted));
                        ImGui::Text("UUID: %s", std::to_string(uuid.value).c_str());
                        ImGui::PopStyleColor();
                    }
                }

                if (entity.HasComponent<TransformComponent>()) {
                    if (Widgets::CollapsingHeader("Transform", Colors::transform)) {
                        auto& [position, scale, rotation] = entity.GetComponent<TransformComponent>();
                        ImGui::DragFloat2("Position", &position[0], 0.1f);
                        ImGui::DragFloat2("Scale", &scale[0], 0.1f);
                        ImGui::DragFloat("Rotation", &rotation, 0.1f);
                    }
                }

                if (entity.HasComponent<SpriteComponent>()) {
                    if (Widgets::CollapsingHeader("Sprite", Colors::sprite)) {
                        auto& [tint, uuid] = entity.GetComponent<SpriteComponent>();
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

                if (entity.HasComponent<ScriptComponent>()) {
                    if (Widgets::CollapsingHeader("Script", Colors::script)) {
                        auto& [uuid, _] = entity.GetComponent<ScriptComponent>();
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
        Widgets::End();
    }
} // namespace Cobalt

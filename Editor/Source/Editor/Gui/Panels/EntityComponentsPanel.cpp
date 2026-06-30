// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Graphics/Texture2D.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Cobalt
{
    auto EntityComponentsPanel::Draw(EditorState& state) -> void {
        Widgets::Begin("Components", {8, 8});
        {
            auto& imgui_style = ImGui::GetStyle();

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

                        const auto uuid_str = std::to_string(uuid.value);
                        const auto region_avail = ImGui::GetContentRegionAvail();
                        const auto cursor_pos = ImGui::GetCursorScreenPos();
                        const auto draw_list = ImGui::GetWindowDrawList();
                        constexpr auto thumbnail_size = ImVec2{60, 60};
                        constexpr auto thumbnail_padding = 10.0f;
                        constexpr auto rect_height = thumbnail_size.y + thumbnail_padding * 2.0f;

                        ImGui::InvisibleButton(uuid_str.c_str(), {region_avail.x, rect_height});

                        if (ImGui::BeginDragDropTarget()) {
                            if (const auto* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_AND_DROP")) {
                                const u64 uid = *static_cast<u64*>(payload->Data);
                                uuid.value = uid;
                            }
                            ImGui::EndDragDropTarget();
                        }

                        draw_list->AddRectFilled(
                            cursor_pos,
                            cursor_pos + ImVec2{region_avail.x, rect_height},
                            ImGui::GetColorU32(imgui_style.Colors[ImGuiCol_FrameBg]),
                            imgui_style.FrameRounding
                        );

                        const auto texture = AssetManager::Get().GetAsset<Texture2D>(uuid);
                        const auto text_line_height = ImGui::GetTextLineHeight();
                        const auto text_y_offset = rect_height * 0.5f - text_line_height * 0.5f;
                        if (texture) {
                            ImGui::SetCursorScreenPos(cursor_pos + ImVec2{thumbnail_padding, thumbnail_padding});
                            ImGui::Image(texture->GetRendererID(), thumbnail_size);

                            ImGui::SetCursorScreenPos(cursor_pos + ImVec2{thumbnail_size.x + thumbnail_padding * 2, text_y_offset});
                            const auto meta = AssetManager::Get().GetRegistry().GetMetadata(uuid).value();
                            const auto texture_name_string = meta.path.filename().string();
                            ImGui::Text("%s", texture_name_string.c_str());
                        } else {
                            const auto min = cursor_pos + ImVec2{thumbnail_padding, thumbnail_padding};
                            draw_list->AddRectFilled(
                                min,
                                min + thumbnail_size,
                                IM_COL32_WHITE
                                );

                            ImGui::SetCursorScreenPos(cursor_pos + ImVec2{thumbnail_size.x + thumbnail_padding * 2, text_y_offset});
                            ImGui::Text("Default Texture");
                        }

                        ImGui::SetCursorScreenPos(cursor_pos);
                        ImGui::Dummy({0, rect_height});
                    }
                }

                if (entity.HasComponent<ScriptComponent>()) {
                    if (Widgets::CollapsingHeader("Script", Colors::script)) {
                        auto& [uuid, _] = entity.GetComponent<ScriptComponent>();

                        const auto uuid_str = std::to_string(uuid.value);
                        const auto region_avail = ImGui::GetContentRegionAvail();
                        const auto cursor_pos = ImGui::GetCursorScreenPos();
                        const auto draw_list = ImGui::GetWindowDrawList();
                        constexpr auto thumbnail_padding = 10.0f;
                        const auto text_line_height = ImGui::GetTextLineHeight();
                        const auto rect_height = text_line_height + thumbnail_padding * 2.0f;

                        ImGui::InvisibleButton(uuid_str.c_str(), {region_avail.x, rect_height});

                        if (ImGui::BeginDragDropTarget()) {
                            if (const auto* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_AND_DROP")) {
                                const u64 uid = *static_cast<u64*>(payload->Data);
                                uuid.value = uid;
                            }
                            ImGui::EndDragDropTarget();
                        }

                        draw_list->AddRectFilled(
                            cursor_pos,
                            cursor_pos + ImVec2{region_avail.x, rect_height},
                            ImGui::GetColorU32(imgui_style.Colors[ImGuiCol_FrameBg]),
                            imgui_style.FrameRounding
                        );

                        auto class_name = String{};
                        if (auto meta_opt = AssetManager::Get().GetRegistry().GetMetadata(uuid)) {
                            auto& meta = meta_opt.value();
                            class_name = meta.path.filename().string();
                        } else {
                            class_name = "None";
                        }

                        ImGui::SetCursorScreenPos(cursor_pos + ImVec2(thumbnail_padding, thumbnail_padding));
                        ImGui::Text("Script Class: %s", class_name.c_str());

                        ImGui::SetCursorScreenPos(cursor_pos);
                        ImGui::Dummy({0, rect_height});
                    }
                }
            }
        }
        Widgets::End();
    }
} // namespace Cobalt

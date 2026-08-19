// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/AssetEditors/SceneEditor.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <imgui.h>
#include <imgui_internal.h>

// IMPORTANT: Include ImGuizmo after imgui.h
#include <ImGuizmo.h>

namespace Cobalt
{
    inline auto GizmoOperationToImGuizmo(const GizmoOperation operation) -> ImGuizmo::OPERATION {
        switch (operation) {
            case GizmoOperation::Universal: return ImGuizmo::UNIVERSAL;
            case GizmoOperation::Translate: return ImGuizmo::TRANSLATE;
            case GizmoOperation::Rotate: return ImGuizmo::ROTATE;
            case GizmoOperation::Scale: return ImGuizmo::SCALE;
        }

        return ImGuizmo::UNIVERSAL;
    }

    SceneEditor::SceneEditor(const UUID& asset_uuid) : AssetEditor(asset_uuid) {
        SetName("Scene Editor");
    }

    void SceneEditor::OnInitLayout(ImGuiID dockspace_id) {
        ImGui::DockBuilderRemoveNode(dockspace_id);

        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

        ImGuiID viewport_node;
        ImGuiID right_sidebar_node;
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, &right_sidebar_node, &viewport_node);

        ImGuiID hierarchy_node;
        ImGuiID components_node;
        ImGui::DockBuilderSplitNode(right_sidebar_node, ImGuiDir_Up, 0.50f, &hierarchy_node, &components_node);

        if (const auto node = ImGui::DockBuilderGetNode(viewport_node)) {
            node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
        }

        ImGui::DockBuilderDockWindow("Viewport", viewport_node);
        ImGui::DockBuilderDockWindow("Scene Hierarchy", hierarchy_node);
        ImGui::DockBuilderDockWindow("Components", components_node);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    void SceneEditor::OnUpdate(EditorState& state) {
        AssetEditor::OnUpdate(state);
    }

    void SceneEditor::OnDrawPanels(EditorState& state) {
        DrawViewport(state);
        DrawHierarchy(state);
        DrawComponents(state);
    }

    auto SceneEditor::DrawViewport(EditorState& state) const -> void {
        const auto window_class = GetWindowClass();
        ImGui::SetNextWindowClass(&window_class);
        ImGui::Begin("Viewport", nullptr, 0);
        {
            static auto mode = ImGuizmo::LOCAL;
            static auto should_snap = false;
            f32 snap_amount = 0.0f;

            if (ImGui::IsWindowFocused()) {
                if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                    state.gizmo_operation = GizmoOperation::Universal;
                } else if (ImGui::IsKeyPressed(ImGuiKey_W)) {
                    state.gizmo_operation = GizmoOperation::Translate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_E)) {
                    state.gizmo_operation = GizmoOperation::Rotate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_R)) {
                    state.gizmo_operation = GizmoOperation::Scale;
                }

                if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                    should_snap = true;
                } else if (ImGui::IsKeyReleased(ImGuiKey_LeftShift)) {
                    should_snap = false;
                }
            }

            static auto toolbar_pos = ImVec2();
            toolbar_pos = ImGui::GetCursorScreenPos();

            if (const auto color_id = state.framebuffer.GetColorAttachmentID(0); color_id >= 0) {
                const auto viewport_size = ImGui::GetContentRegionAvail();
                state.framebuffer.Resize(viewport_size.x, viewport_size.y);
                ImGui::Image(color_id, viewport_size, {0, 1}, {1, 0});
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});

            ImGui::SetCursorScreenPos(toolbar_pos);
            if (ImGui::RadioButton("Universal", state.gizmo_operation == GizmoOperation::Universal)) {
                state.gizmo_operation = GizmoOperation::Universal;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Translate", state.gizmo_operation == GizmoOperation::Translate)) {
                state.gizmo_operation = GizmoOperation::Translate;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", state.gizmo_operation == GizmoOperation::Rotate)) {
                state.gizmo_operation = GizmoOperation::Rotate;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", state.gizmo_operation == GizmoOperation::Scale)) {
                state.gizmo_operation = GizmoOperation::Scale;
            }
            ImGui::SameLine();
            ImGui::Dummy({20, 0});
            ImGui::SameLine();
            if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL)) {
                mode = ImGuizmo::LOCAL;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD)) {
                mode = ImGuizmo::WORLD;
            }

            ImGui::PopStyleVar();

            if (state.selected_entity != entt::null && SceneManager::Get().GetState() == SceneState::None) {
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

                auto entity = Entity(state.selected_entity, &state.active_scene->GetRegistry());
                auto& transform_component = entity.GetComponent<TransformComponent>();
                auto transform_matrix = transform_component.TransformMatrix();

                if (should_snap) {
                    // TODO: Implement snapping
                } else {
                    snap_amount = 0.0f;
                }

                ImGuizmo::Manipulate(glm::value_ptr(state.editor_camera.GetView()),
                                     glm::value_ptr(state.editor_camera.GetProjection(state.framebuffer.GetSize())),
                                     GizmoOperationToImGuizmo(state.gizmo_operation), mode, glm::value_ptr(transform_matrix), nullptr,
                                     should_snap ? &snap_amount : nullptr);

                if (ImGuizmo::IsUsing()) {
                    Vec3 scale;
                    Quat rotation;
                    Vec3 position;
                    Vec3 skew;
                    Vec4 perspective;
                    glm::decompose(transform_matrix, scale, rotation, position, skew, perspective);

                    switch (state.gizmo_operation) {
                        case GizmoOperation::Universal:
                            transform_component.position = {position.x, position.y};
                            transform_component.rotation = glm::degrees(glm::eulerAngles(rotation)).z;
                            transform_component.scale = {scale.x, scale.y};
                            break;
                        case GizmoOperation::Translate: transform_component.position = {position.x, position.y}; break;
                        case GizmoOperation::Rotate: transform_component.rotation = glm::degrees(glm::eulerAngles(rotation)).z; break;
                        case GizmoOperation::Scale: transform_component.scale = {scale.x, scale.y}; break;
                        default:;
                    }
                }
            }
        }
        ImGui::End();
    }

    auto SceneEditor::DrawHierarchy(EditorState& state) const -> void {
        const auto window_class = GetWindowClass();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
        ImGui::SetNextWindowClass(&window_class);
        ImGui::Begin("Scene Hierarchy");
        {
            // Fix annoyance of have to click into the viewport in order to change the gizmo operation
            if (ImGui::IsWindowFocused()) {
                if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                    state.gizmo_operation = GizmoOperation::Universal;
                } else if (ImGui::IsKeyPressed(ImGuiKey_W)) {
                    state.gizmo_operation = GizmoOperation::Translate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_E)) {
                    state.gizmo_operation = GizmoOperation::Rotate;
                } else if (ImGui::IsKeyPressed(ImGuiKey_R)) {
                    state.gizmo_operation = GizmoOperation::Scale;
                }
            }

            auto index = 0;
            const auto view = state.active_scene->GetRegistry().view<entt::entity>();
            for (auto it = view.rbegin(); it != view.rend(); ++it) {
                const auto entity = *it;
                auto [name, id] = state.active_scene->GetRegistry().get<TagComponent>(entity);

                ImGui::PushID(id.value);

                constexpr auto padding = 10.0f;
                const auto content_avail = ImGui::GetContentRegionAvail();
                const auto title_height = ImGui::GetTextLineHeightWithSpacing();
                const auto min = ImGui::GetCursorScreenPos();
                const auto max = ImVec2{min.x + content_avail.x, min.y + title_height + padding * 2.0f};
                const auto size = max - min;

                auto bg_col = index++ & 1 ? Colors::node_odd : Colors::node_even;

                ImGui::PushID(index * 8127812 % 23);
                const auto clicked = ImGui::InvisibleButton("##Node", size);
                const auto hovered = ImGui::IsItemHovered();
                ImGui::PopID();

                if (clicked) {
                    state.selected_entity = entity;
                }

                if (state.selected_entity == entity) {
                    bg_col = Colors::node_active;
                }

                bg_col = hovered ? Colors::node_hovered : bg_col;

                ImGui::RenderFrame(min, max, ImGui::GetColorU32(IMVEC4(bg_col)), false);

                ImGui::SetCursorScreenPos({min.x + padding, min.y + padding});
                ImGui::Text("%s", name.c_str());

                ImGui::SetCursorScreenPos(min);
                ImGui::Dummy(size);

                ImGui::PopID();
            }

            if (ImGui::IsWindowHovered()) {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    state.selected_entity = entt::null;
                }

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                    ImGui::OpenPopup("Scene Hierarchy Context Menu");
                }
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});
            if (ImGui::BeginPopupContextItem("Scene Hierarchy Context Menu")) {
                ImGui::SeparatorText("Create");
                if (Widgets::Button("Empty Entity")) {
                    const auto entity = state.active_scene->CreateEntity("Entity");
                    state.selected_entity = entity.ID();

                    ImGui::CloseCurrentPopup();
                }
                if (Widgets::Button("Sprite Entity")) {
                    auto entity = state.active_scene->CreateEntity("Sprite");
                    entity.AddComponent<SpriteComponent>();
                    state.selected_entity = entity.ID();

                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
            ImGui::PopStyleVar(2);
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    auto SceneEditor::DrawComponents(EditorState& state) const -> void {
        const auto window_class = GetWindowClass();
        ImGui::SetNextWindowClass(&window_class);
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
                        Widgets::TextInput("Name", &name);

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
                            ImGui::Image(texture->GetRendererID(), thumbnail_size, {0, 1}, {1, 0});

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
                            class_name = meta.path.filename().string().c_str();
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

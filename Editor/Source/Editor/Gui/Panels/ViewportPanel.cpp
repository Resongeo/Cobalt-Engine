// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <imgui.h>

// IMPORTANT: Include ImGuizmo after imgui.h
#include <ImGuizmo.h>

namespace Cobalt
{
    inline auto gizmo_operation_to_imguizmo(const GizmoOperation operation) -> ImGuizmo::OPERATION {
        switch (operation) {
            case GizmoOperation::Universal: return ImGuizmo::UNIVERSAL;
            case GizmoOperation::Translate: return ImGuizmo::TRANSLATE;
            case GizmoOperation::Rotate: return ImGuizmo::ROTATE;
            case GizmoOperation::Scale: return ImGuizmo::SCALE;
        }

        return ImGuizmo::UNIVERSAL;
    }

    auto ViewportPanel::Draw(EditorState& state) -> void {
        Widgets::Begin("Viewport");
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
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(),
                                  ImGui::GetWindowHeight());

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
                                     gizmo_operation_to_imguizmo(state.gizmo_operation), mode,
                                     glm::value_ptr(transform_matrix), nullptr, should_snap ? &snap_amount : nullptr);

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
                        case GizmoOperation::Translate:
                            transform_component.position = {position.x, position.y};
                            break;
                        case GizmoOperation::Rotate:
                            transform_component.rotation = glm::degrees(glm::eulerAngles(rotation)).z;
                            break;
                        case GizmoOperation::Scale: transform_component.scale = {scale.x, scale.y}; break;
                        default:;
                    }
                }
            }
        }
        Widgets::End();
    }
} // namespace Cobalt

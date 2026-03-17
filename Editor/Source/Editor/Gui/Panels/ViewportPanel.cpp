// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Entity.hpp"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Cobalt::Editor
{
    auto ViewportPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Viewport");
        {
            static auto operation = ImGuizmo::TRANSLATE;
            static auto mode = ImGuizmo::LOCAL;
            static auto should_snap = false;
            f32 snap_amount = 0.0f;

            if (ImGui::IsWindowFocused()) {
                if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                    operation = ImGuizmo::TRANSLATE;
                } else if (ImGui::IsKeyPressed(ImGuiKey_W)) {
                    operation = ImGuizmo::ROTATE;
                } else if (ImGui::IsKeyPressed(ImGuiKey_E)) {
                    operation = ImGuizmo::SCALE;
                }

                if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                    should_snap = true;
                } else if (ImGui::IsKeyReleased(ImGuiKey_LeftShift)) {
                    should_snap = false;
                }
            }

            if (ImGui::RadioButton("Translate", operation == ImGuizmo::TRANSLATE)) {
                operation = ImGuizmo::TRANSLATE;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", operation == ImGuizmo::ROTATE)) {
                operation = ImGuizmo::ROTATE;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", operation == ImGuizmo::SCALE)) {
                operation = ImGuizmo::SCALE;
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

            if (const auto color_id = state.framebuffer.get_color_attachment_id(0); color_id >= 0) {
                const auto viewport_size = ImGui::GetContentRegionAvail();
                state.framebuffer.resize(viewport_size.x, viewport_size.y);
                ImGui::Image(color_id, viewport_size, {0, 1},{1, 0});
            }   

            if (state.selected_entity != entt::null) {
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(
                    ImGui::GetWindowPos().x,
                    ImGui::GetWindowPos().y,
                    ImGui::GetWindowWidth(),
                    ImGui::GetWindowHeight()
                );

                auto entity = Engine::Entity(state.selected_entity, &state.active_scene->registry());
                auto& transform_component = entity.get_component<Engine::TransformComponent>();
                auto transform_matrix = transform_component.get_transform_matrix();

                if (should_snap) {
                    switch (operation) {
                        case ImGuizmo::TRANSLATE:
                            snap_amount = 0.5f;
                            break;
                        case ImGuizmo::ROTATE:
                            snap_amount = 15.0f;
                            break;
                        case ImGuizmo::SCALE:
                            snap_amount = 0.1f;
                            break;
                        default: break;
                    }
                } else {
                    snap_amount = 0.0f;
                }
                
                ImGuizmo::Manipulate(
                    glm::value_ptr(state.editor_camera.view()),
                    glm::value_ptr(state.editor_camera.projection(state.framebuffer.get_size())),
                    operation,
                    mode,
                    glm::value_ptr(transform_matrix),
                    nullptr,
                    should_snap ? &snap_amount : nullptr
                );

                if (ImGuizmo::IsUsing())
                {
                    Vec3 scale;
                    Quat rotation;
                    Vec3 position;
                    Vec3 skew;
                    Vec4 perspective;
                    glm::decompose(transform_matrix, scale, rotation, position, skew, perspective);

                    switch (operation)
                    {
                        case ImGuizmo::OPERATION::TRANSLATE:
                            transform_component.position = {position.x, position.y};
                            break;
                        case ImGuizmo::ROTATE:
                            transform_component.rotation = glm::degrees(glm::eulerAngles(rotation)).z;
                            break;
                        case ImGuizmo::OPERATION::SCALE:
                            transform_component.scale = {scale.x, scale.y};
                            break;
                        default:;
                    }
                }
            }
        }
        ImGui::End();
    }
}

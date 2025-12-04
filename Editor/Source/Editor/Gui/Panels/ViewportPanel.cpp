// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

#include "glm/gtx/matrix_decompose.hpp"

namespace Cobalt::Editor
{
    auto ViewportPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Viewport");
        {
            static auto operation = ImGuizmo::TRANSLATE;
            static auto mode = ImGuizmo::LOCAL;

            if (ImGui::IsWindowFocused()) {
                if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
                    operation = ImGuizmo::TRANSLATE;
                } else if (ImGui::IsKeyPressed(ImGuiKey_W)) {
                    operation = ImGuizmo::ROTATE;
                } else if (ImGui::IsKeyPressed(ImGuiKey_E)) {
                    operation = ImGuizmo::SCALE;
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
                
                auto& [p, s] = state.active_scene->registry().get<Engine::TransformComponent>(state.selected_entity);
                auto transform = glm::translate(Mat4(1.0f), {p.x, p.y, 0.0})
                    * glm::scale(Mat4(1.0f), {s.x, s.y, 1.0f});
                
                ImGuizmo::Manipulate(
                    glm::value_ptr(state.editor_camera.view()),
                    glm::value_ptr(state.editor_camera.projection(state.framebuffer.get_size())),
                    operation,
                    mode,
                    glm::value_ptr(transform)
                );

                if (ImGuizmo::IsUsing())
                {
                    Vec3 scale;
                    Quat rotation;
                    Vec3 translation;
                    Vec3 skew;
                    Vec4 perspective;
                    glm::decompose(transform, scale, rotation, translation, skew, perspective);

                    switch (operation)
                    {
                        case ImGuizmo::OPERATION::TRANSLATE:
                            p = {translation.x, translation.y};
                            break;
                        case ImGuizmo::OPERATION::SCALE:
                            s = {scale.x, scale.y};
                            break;
                        default:;
                    }
                }
            }
        }
        ImGui::End();
    }
}

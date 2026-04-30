// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/Colors.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Entity.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Cobalt
{
    auto SceneHierarchyPanel::draw(EngineContext& ctx, EditorState& state) -> void {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

        Widgets::begin("Scene Hierarchy");
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
            const auto view = state.active_scene->get_registry().view<entt::entity>();
            for (auto it = view.rbegin(); it != view.rend(); ++it) {
                const auto entity = *it;
                auto [name, id] = state.active_scene->get_registry().get<TagComponent>(entity);

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

            if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                state.selected_entity = entt::null;
            }
        }
        Widgets::end();

        ImGui::PopStyleVar();
    }
} // namespace Cobalt

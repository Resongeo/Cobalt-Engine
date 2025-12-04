// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Gui/Panels/ViewportPanel.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    auto ViewportPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Viewport");
        {
            if (const auto color_id = state.framebuffer.get_color_attachment_id(0); color_id >= 0) {
                const auto framebuffer_size = state.framebuffer.get_size();
                const auto viewport_size = ImGui::GetContentRegionAvail();
                state.framebuffer.resize(viewport_size.x, viewport_size.y);
                ImGui::Image(color_id, viewport_size, {0, 1},{1, 0});
            }   
        }
        ImGui::End();
    }
}

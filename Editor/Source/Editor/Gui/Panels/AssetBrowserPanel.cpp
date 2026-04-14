// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Engine/Core/Project.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    void AssetBrowserPanel::begin(EditorState& state) {
        m_assets_base_dir = Engine::Project::get_project_assets_path();
        m_current_dir = m_assets_base_dir;
    }

    auto AssetBrowserPanel::draw(EditorState& state) -> void {
        ImGui::Begin("Asset Browser");
        {
            if (m_current_dir != std::filesystem::path(m_assets_base_dir)) {
                if (ImGui::Button("<-")) {
                    m_current_dir = m_current_dir.parent_path();
                }
            }

            static auto padding = 16.0f;
            static auto thumbnail_size = 64.0f;
            const auto cell_size = thumbnail_size + padding;

            const auto panel_width = ImGui::GetContentRegionAvail().x;
            auto columnCount = static_cast<i32>(panel_width / cell_size);
            if (columnCount < 1) {
                columnCount = 1;
            }

            ImGui::Columns(columnCount, nullptr, false);

            for (auto& directory_entry : std::filesystem::directory_iterator(m_current_dir)) {
                const auto& path = directory_entry.path();
                auto filename_string = path.filename().string();

                ImGui::PushID(filename_string.c_str());

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 100));
                ImGui::Button(filename_string.c_str(), {thumbnail_size, thumbnail_size});
                ImGui::PopStyleColor();

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    if (directory_entry.is_directory()) {
                        m_current_dir /= path.filename();
                    }
                }

                ImGui::NextColumn();

                ImGui::PopID();
            }

            ImGui::Columns(1);

            ImGui::SliderFloat("Thumbnail Size", &thumbnail_size, 16, 512);
            ImGui::SliderFloat("Padding", &padding, 0, 32);
        }
        ImGui::End();
    }
} // namespace Cobalt::Editor

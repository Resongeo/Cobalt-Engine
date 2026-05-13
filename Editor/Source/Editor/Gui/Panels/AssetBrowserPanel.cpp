// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Logger.hpp"

#include <imgui.h>

namespace Cobalt
{
    constexpr float THUMBNAIL_MIN_SIZE = 70.0f;
    constexpr float THUMBNAIL_MAX_SIZE = 128.0f;

    void AssetBrowserPanel::begin(EngineContext& ctx, EditorState& state) {
        m_assets_base_dir = ctx.project.get_project_assets_path();
        m_current_dir = m_assets_base_dir;
    }

    auto AssetBrowserPanel::draw(EngineContext& ctx, EditorState& state) -> void {
        Widgets::begin("Asset Browser", {8, 8});
        {
            if (m_current_dir != std::filesystem::path(m_assets_base_dir)) {
                if (Widgets::button("<-")) {
                    m_current_dir = m_current_dir.parent_path();
                }
            }

            if (Widgets::button("Refresh")) {
                for (auto& entry : std::filesystem::directory_iterator(m_current_dir)) {
                    if (!entry.is_directory()) {
                        ctx.asset_manager.register_asset(entry.path());
                    }
                }
            }

            static auto padding = 8.0f;
            static auto thumbnail_size = 80.0f;
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
                Widgets::button(filename_string.c_str(), Variant::Default, {thumbnail_size, thumbnail_size});
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

            if (const auto& io = ImGui::GetIO(); io.MouseWheel != 0 && io.KeyCtrl && ImGui::IsWindowHovered()) {
                thumbnail_size += 2.0f * io.MouseWheel;

                if (thumbnail_size < THUMBNAIL_MIN_SIZE) {
                    thumbnail_size = THUMBNAIL_MIN_SIZE;
                } else if (thumbnail_size > THUMBNAIL_MAX_SIZE) {
                    thumbnail_size = THUMBNAIL_MAX_SIZE;
                }

                Logger::warn("", "{}", thumbnail_size);
            }
        }
        Widgets::end();
    }
} // namespace Cobalt

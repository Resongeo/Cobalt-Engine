// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/Colors.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Logger.hpp"

#include <imgui.h>

namespace Cobalt
{
    constexpr float THUMBNAIL_MIN_SIZE = 70.0f;
    constexpr float THUMBNAIL_MAX_SIZE = 150.0f;

    void AssetBrowserPanel::begin(EngineContext& ctx, EditorState& state) {
        m_assets_base_dir = ctx.project.get_project_assets_path();
        m_current_dir = m_assets_base_dir;

        const auto editor_asset_path = ctx.project.get_editor_assets_path();

        m_directory_texture = Memory::make_rc<Texture2D>();
        m_directory_texture->load_from_file(editor_asset_path / "Textures" / "Directory.png");

        m_default_texture = Memory::make_rc<Texture2D>();
        m_default_texture->load_from_file(editor_asset_path / "Textures" / "Default.png");

        m_script_texture = Memory::make_rc<Texture2D>();
        m_script_texture->load_from_file(editor_asset_path / "Textures" / "Script.png");

        m_texture_texture = Memory::make_rc<Texture2D>();
        m_texture_texture->load_from_file(editor_asset_path / "Textures" / "Texture.png");
    }

    auto AssetBrowserPanel::draw(EngineContext& ctx, EditorState& state) -> void {
        Widgets::begin("Asset Browser", {8, 8});
        {
            ImGui::PushFont(Fonts::icon);

            if (Widgets::button(ICON_ARROW_LEFT, Variant::Default, {0, 0}, true)) {
                if (m_current_dir != std::filesystem::path(m_assets_base_dir)) {
                    m_current_dir = m_current_dir.parent_path();
                    m_directory_changed = true;
                }
            }

            ImGui::SameLine();

            if (Widgets::button(ICON_REFRESH, Variant::Default, {0, 0}, true) || m_directory_changed) {
                for (auto& entry : std::filesystem::directory_iterator(m_current_dir)) {
                    if (!entry.is_directory()) {
                        ctx.asset_manager.register_asset(entry.path());
                    }
                }

                m_directory_changed = false;
            }

            ImGui::PopFont();

            static auto padding = 8.0f;
            static auto thumbnail_size = 100.0f;
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

                const auto texture = get_texture_from_dir_entry(directory_entry);
                auto tint_col = ImVec4{1, 1, 1, 1};

                if (directory_entry.is_directory()) {
                    if (m_directory_colors.contains(path)) {
                        tint_col = IMVEC4(m_directory_colors[path]);
                    } else {
                        tint_col = IMVEC4(Colors::directory);
                    }
                }

                auto cursor_pos = ImGui::GetCursorScreenPos();
                ImGui::InvisibleButton(filename_string.c_str(), {thumbnail_size, thumbnail_size});

                if (ImGui::IsItemHovered()) {
                    tint_col *= 0.9f;

                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        if (directory_entry.is_directory()) {
                            m_current_dir /= path.filename();
                            m_directory_changed = true;
                        }
                    }

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                        ImGui::OpenPopup("Asset Context Menu");
                    }
                }

                ImGui::SetCursorScreenPos(cursor_pos);
                ImGui::ImageWithBg(
                    texture->get_renderer_id(),
                    {thumbnail_size, thumbnail_size},
                    {0, 1}, {1, 0},
                    {0, 0, 0, 0}, tint_col
                );
                ImGui::TextWrapped("%s", filename_string.c_str());

                ImGui::NextColumn();
            }

            ImGui::Columns(1);

            if (const auto& io = ImGui::GetIO(); io.MouseWheel != 0 && io.KeyCtrl && ImGui::IsWindowHovered()) {
                thumbnail_size += 2.0f * io.MouseWheel;

                if (thumbnail_size < THUMBNAIL_MIN_SIZE) {
                    thumbnail_size = THUMBNAIL_MIN_SIZE;
                } else if (thumbnail_size > THUMBNAIL_MAX_SIZE) {
                    thumbnail_size = THUMBNAIL_MAX_SIZE;
                }
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
            if (ImGui::BeginPopupContextItem("Asset Context Menu")) {
                if (Widgets::button("Change Color")) {
                    Logger::warn("Asset Browser", "Pressed Change Color");
                }

                ImGui::EndPopup();
            }
            ImGui::PopStyleVar(1);
        }
        Widgets::end();
    }

    auto AssetBrowserPanel::get_texture_from_dir_entry(const std::filesystem::directory_entry& entry) const
            -> Texture2D* {
        if (entry.is_directory()) {
            return m_directory_texture.get();
        }

        switch (AssetManager::get_asset_type_from_extension(entry.path())) {
            case AssetType::Texture: return m_texture_texture.get();
            case AssetType::Script: return m_script_texture.get();
            case AssetType::Scene:
            default: return m_default_texture.get();
        }

        return m_default_texture.get();
    }
} // namespace Cobalt

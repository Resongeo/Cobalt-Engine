// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Editor/Gui/Colors.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Project.hpp"

#include <imgui.h>

namespace Cobalt
{
    constexpr float THUMBNAIL_MIN_SIZE = 70.0f;
    constexpr float THUMBNAIL_MAX_SIZE = 150.0f;

    void AssetBrowserPanel::Begin(EngineContext& ctx, EditorState& state) {
        _assets_base_dir = Project::Get().GetProjectAssetsPath();
        _current_dir = _assets_base_dir;

        const auto editor_asset_path = Project::Get().GetEditorAssetsPath();

        _directory_texture = Memory::MakeRc<Texture2D>();
        _directory_texture->LoadFromFile(editor_asset_path / "Textures" / "Directory.png");

        _default_texture = Memory::MakeRc<Texture2D>();
        _default_texture->LoadFromFile(editor_asset_path / "Textures" / "Default.png");

        _script_texture = Memory::MakeRc<Texture2D>();
        _script_texture->LoadFromFile(editor_asset_path / "Textures" / "Script.png");

        _texture_texture = Memory::MakeRc<Texture2D>();
        _texture_texture->LoadFromFile(editor_asset_path / "Textures" / "Texture.png");
    }

    auto AssetBrowserPanel::Draw(EngineContext& ctx, EditorState& state) -> void {
        Widgets::Begin("Asset Browser", {8, 8});
        {
            ImGui::PushFont(Fonts::icon);

            if (Widgets::Button(ICON_ARROW_LEFT, Variant::Default, {0, 0}, true)) {
                if (_current_dir != std::filesystem::path(_assets_base_dir)) {
                    _current_dir = _current_dir.parent_path();
                    _directory_changed = true;
                }
            }

            ImGui::SameLine();

            if (Widgets::Button(ICON_REFRESH, Variant::Default, {0, 0}, true) || _directory_changed) {
                for (auto& entry : std::filesystem::directory_iterator(_current_dir)) {
                    if (!entry.is_directory()) {
                        AssetManager::Get().RegisterAsset(entry.path());
                    }
                }

                _directory_changed = false;
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

            for (auto& directory_entry : std::filesystem::directory_iterator(_current_dir)) {
                const auto& path = directory_entry.path();
                auto filename_string = path.filename().string();

                const auto texture = GetTextureFromDirEntry(directory_entry);
                auto tint_col = ImVec4{1, 1, 1, 1};

                if (directory_entry.is_directory()) {
                    if (_directory_colors.contains(path)) {
                        tint_col = IMVEC4(_directory_colors[path]);
                    } else {
                        tint_col = IMVEC4(Colors::directory);
                    }
                }

                auto cursor_pos = ImGui::GetCursorScreenPos();
                ImGui::InvisibleButton(filename_string.c_str(), {thumbnail_size, thumbnail_size});

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    auto uuid = AssetManager::Get().GetUUID(path);
                    ImGui::SetDragDropPayload("ASSET_DRAG_AND_DROP", &uuid.value, sizeof(u64));
                    ImGui::EndDragDropSource();
                }

                if (ImGui::IsItemHovered()) {
                    tint_col *= 0.9f;

                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        if (directory_entry.is_directory()) {
                            _current_dir /= path.filename();
                            _directory_changed = true;
                        }
                    }

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                        ImGui::OpenPopup("Asset Context Menu");
                    }
                }

                ImGui::SetCursorScreenPos(cursor_pos);
                ImGui::ImageWithBg(
                    texture->GetRendererID(),
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
                if (Widgets::Button("Change Color")) {
                    CORE_INFO("Editor::AssetBrowser: Pressed Change Color");
                }

                ImGui::EndPopup();
            }
            ImGui::PopStyleVar(1);
        }
        Widgets::End();
    }

    auto AssetBrowserPanel::GetTextureFromDirEntry(const std::filesystem::directory_entry& entry) const
            -> Texture2D* {
        if (entry.is_directory()) {
            return _directory_texture.get();
        }

        switch (AssetManager::GetAssetTypeFromExtension(entry.path())) {
            case AssetType::Texture: return _texture_texture.get();
            case AssetType::Script: return _script_texture.get();
            case AssetType::Scene:
            default: return _default_texture.get();
        }
    }
} // namespace Cobalt

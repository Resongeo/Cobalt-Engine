// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Gui/AssetEditors/AssetEditor.hpp"

namespace Cobalt
{
    class AssetBrowserPanel;
    class Texture2D;

    class SceneEditor final : public AssetEditor
    {
    public:
        explicit SceneEditor(const UUID& asset_uuid);

        auto OnInitLayout(ImGuiID dockspace_id) -> void override;
        auto OnUpdate(EditorState& state) -> void override;
        auto OnDrawPanels(EditorState& state) -> void override;

    private:
        auto DrawViewport(EditorState& state) const -> void;
        auto DrawHierarchy(EditorState& state) const -> void;
        auto DrawComponents(EditorState& state) const -> void;
        auto DrawAssetsBrowser(EditorState& state) -> void;

        auto GetTextureFromDirEntry(const std::filesystem::directory_entry& entry) const -> Texture2D*;

    private:
        Filepath _assets_base_dir = {};
        Filepath _current_dir = {};
        HashMap<Filepath, Color> _directory_colors = {};
        bool _directory_changed = false;
    };
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Gui/IPanel.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Graphics/Texture2D.hpp"

namespace Cobalt
{
    class AssetBrowserPanel final : public IPanel
    {
    public:
        auto Begin(EditorState& state) -> void override;
        auto Draw(EditorState& state) -> void override;

    private:
        auto GetTextureFromDirEntry(const std::filesystem::directory_entry& entry) const -> Texture2D*;

    private:
        Filepath _assets_base_dir = {};
        Filepath _current_dir = {};
        bool _directory_changed = false;
        HashMap<Filepath, Color> _directory_colors = {};
        Rc<Texture2D> _directory_texture = {};
        Rc<Texture2D> _texture_texture = {};
        Rc<Texture2D> _script_texture = {};
        Rc<Texture2D> _default_texture = {};
    };
} // namespace Cobalt

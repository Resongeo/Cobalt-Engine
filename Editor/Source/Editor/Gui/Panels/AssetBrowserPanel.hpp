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
        auto begin(EngineContext& ctx, EditorState& state) -> void override;
        auto draw(EngineContext& ctx, EditorState& state) -> void override;

    private:
        auto get_texture_from_dir_entry(const std::filesystem::directory_entry& entry) const -> Texture2D*;

    private:
        Filepath m_assets_base_dir = {};
        Filepath m_current_dir = {};
        bool m_directory_changed = false;
        HashMap<Filepath, Color> m_directory_colors = {};
        Rc<Texture2D> m_directory_texture = {};
        Rc<Texture2D> m_texture_texture = {};
        Rc<Texture2D> m_script_texture = {};
        Rc<Texture2D> m_default_texture = {};
    };
} // namespace Cobalt

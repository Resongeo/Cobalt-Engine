// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Gui/IPanel.hpp"
#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt
{
    class AssetBrowserPanel final : public IPanel
    {
    public:
        auto begin(EngineContext& ctx, EditorState& state) -> void override;
        auto draw(EngineContext& ctx, EditorState& state) -> void override;

    private:
        Filepath m_assets_base_dir = {};
        Filepath m_current_dir = {};
    };
} // namespace Cobalt

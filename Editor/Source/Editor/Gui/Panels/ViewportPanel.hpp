// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Gui/IPanel.hpp"

namespace Cobalt
{
    class ViewportPanel final : public IPanel
    {
    public:
        auto draw(EngineContext& ctx, EditorState& state) -> void override;
    };
} // namespace Cobalt

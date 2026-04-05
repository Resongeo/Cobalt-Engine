// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Gui/IPanel.hpp"

namespace Cobalt::Engine
{
    class ViewportPanel final : public IPanel
    {
    public:
        auto draw(EditorState& state) -> void override;
    };
} // namespace Cobalt::Editor


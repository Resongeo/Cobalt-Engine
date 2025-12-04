// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Editor/Gui/IPanel.hpp"

namespace Cobalt::Editor
{
    class SceneHierarchyPanel final : public IPanel
    {
    public:
        auto draw(EditorState& state) -> void override;
    };
}
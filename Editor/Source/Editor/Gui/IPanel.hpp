// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Core/EditorState.hpp"

namespace Cobalt::Editor
{
    class IPanel
    {
    public:
        virtual ~IPanel() = default;

        virtual auto begin(EditorState& state) -> void {}
        virtual auto draw(EditorState& state) -> void = 0;
    };
} // namespace Cobalt::Editor

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Editor/Core/EditorState.hpp"

namespace Cobalt::Editor
{
    class IPanel
    {
    public:
        virtual ~IPanel() = default;
        virtual auto draw(EditorState& state) -> void = 0;
    };
}
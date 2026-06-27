// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Core/EditorState.hpp"
#include "Engine/Core/EngineContext.hpp"

namespace Cobalt
{
    class IPanel
    {
    public:
        virtual ~IPanel() = default;

        virtual auto Begin(EngineContext& ctx, EditorState& state) -> void {}
        virtual auto Draw(EngineContext& ctx, EditorState& state) -> void = 0;
    };
} // namespace Cobalt

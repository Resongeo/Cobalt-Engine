// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Core/EditorState.hpp"
#include "Editor/Gui/IPanel.hpp"
#include "Engine/Core/Application.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Engine/Graphics/Renderer.hpp"

namespace Cobalt
{
    class EditorApplication final : public Application
    {
    public:
        auto OnBegin(EngineContext& ctx) -> void override;
        auto OnUpdate(EngineContext& ctx) -> void override;

    private:
        Renderer _renderer = {};
        Camera _camera = {};
        EditorState _state = {};
        Vector<Box<IPanel>> _panels = {};
    };
} // namespace Cobalt

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
        auto begin(EngineContext& ctx) -> void override;
        auto update(EngineContext& ctx) -> void override;
        auto on_sdl_event(SDL_Event* event) -> void override;

    private:
        Renderer m_renderer = {};
        Camera m_camera = {};
        EditorState m_state = {};
        Vector<Box<IPanel>> m_panels = {};
    };
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Application.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Editor/Core/EditorState.hpp"
#include "Editor/Gui/IPanel.hpp"

namespace Cobalt::Editor
{
    class EditorApplication final : public Engine::Application
    {
    public:
        EditorApplication(i32 argc, char* argv[]);

        auto on_begin() -> void override;
        auto on_event(SDL_Event* event) -> void override;
        auto on_update() -> void override;
        auto on_end() -> void override;

    private:
        Engine::Renderer m_renderer = {};
        Engine::Camera m_camera = {};
        EditorState m_state = {};
        Vector<Box<IPanel>> m_panels = {};
        int m_argc = {};
        char** m_argv = {};
    };
}
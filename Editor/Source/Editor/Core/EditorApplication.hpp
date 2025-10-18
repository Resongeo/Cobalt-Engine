// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Application.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Editor/Core/Project.hpp"

#include <entt/entt.hpp>

namespace Cobalt::Editor
{
    class EditorApplication final : public Engine::Application
    {
    public:
        EditorApplication(i32 argc, char* argv[]);

        auto on_begin() -> void override;
        auto on_update() -> void override;
        auto on_end() -> void override;

    private:
        auto _draw_project_window() -> void;
        auto _draw_scenes_window() -> void;
        auto _draw_entities_window() -> void;
        auto _draw_components_window() const -> void;

    private:
        Project m_project;
        Engine::Renderer m_renderer = {};
        Engine::Camera m_camera = {};
        entt::entity m_selected_entity = entt::null;
    };
}
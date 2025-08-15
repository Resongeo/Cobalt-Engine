// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Application.hpp"
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
        auto draw_project_window() -> void;
        auto draw_scenes_window() -> void;
        auto draw_entities_window() -> void;
        auto draw_components_window() -> void;

    private:
        Project m_project;
        entt::entity m_selected_entity = entt::null;
    };
}
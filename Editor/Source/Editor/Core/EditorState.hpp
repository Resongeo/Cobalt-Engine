// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Scene/Scene.hpp"

#include <entt/entity/entity.hpp>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace Cobalt
{
    enum class GizmoOperation
    {
        Universal,
        Translate,
        Rotate,
        Scale
    };

    struct EditorState
    {
        entt::entity selected_entity = entt::null;
        Rc<Scene> active_scene = nullptr;
        GizmoOperation gizmo_operation = {};
        ImGuiWindowClass main_dockspace_window_class = {};
        ImGuiID main_dockspace_id = {};
    };
} // namespace Cobalt

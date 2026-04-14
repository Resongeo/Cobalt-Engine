// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Graphics/Camera.hpp"
#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Scene/Scene.hpp"

#include <entt/entity/entity.hpp>

namespace Cobalt
{
    struct EditorState
    {
        entt::entity selected_entity = entt::null;
        Scene* active_scene = nullptr;
        Camera editor_camera = {};
        Framebuffer framebuffer = {};
    };
} // namespace Cobalt

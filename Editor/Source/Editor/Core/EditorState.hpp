// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Scene/Scene.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Engine/Graphics/Framebuffer.hpp"

#include <entt/entity/entity.hpp>

namespace Cobalt::Editor
{
    struct EditorState
    {
        entt::entity selected_entity = entt::null;
        Engine::Scene* active_scene = nullptr;
        Engine::Camera editor_camera = {};
        Engine::Framebuffer framebuffer = {};
    };
}
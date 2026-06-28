// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Platform/DialogManager.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

#include <entt/signal/dispatcher.hpp>

namespace Cobalt
{
    struct EngineContext
    {
        SceneManager scene_manager = {};
        ScriptManager script_manager = {};
        DialogManager dialog_manager = {};

        entt::dispatcher dispatcher = {};

        f32 delta_time = 0.0f;
        bool close_requested = false;
    };
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Project.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Scene/SceneManager.hpp"

namespace Cobalt
{
    struct EngineContext
    {
        Project project = {};
        Window window = {};

        SceneManager scene_manager = {};

        bool close_requested = false;
    };
} // namespace Cobalt

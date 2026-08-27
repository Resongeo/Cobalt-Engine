// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Graphics/Camera.hpp"

namespace Cobalt
{
    struct CameraComponent
    {
        Camera camera;
        bool primary = false;
    };
} // namespace Cobalt

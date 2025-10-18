// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"

namespace Cobalt::Engine
{
    struct TransformComponent
    {
        Vec2 position = {0, 0};
        Vec2 scale = {1, 1};
    };
}
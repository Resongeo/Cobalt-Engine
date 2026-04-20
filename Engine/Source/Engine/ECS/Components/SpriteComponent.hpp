// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Color.hpp"
#include "Engine/Core/Types/UUID.hpp"

namespace Cobalt
{
    struct SpriteComponent
    {
        Color tint;
        UUID texture_id;
    };
} // namespace Cobalt

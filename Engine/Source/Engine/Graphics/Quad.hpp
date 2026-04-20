// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"

namespace Cobalt
{
    struct QuadVertexData
    {
        Vec2 position;
        Vec2 tex_coords;
        Vec4 color;
        f32  tex_index;
    };

    struct QuadInstanceData
    {
        Vec2 position;
        Vec4 color;
        f32  texture_id;
    };

    struct QuadDrawCommand
    {
        QuadInstanceData quad;
        i32 order;
    };
} // namespace Cobalt

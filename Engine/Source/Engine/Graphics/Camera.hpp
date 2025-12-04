// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"
#include "Engine/Core/Types/Color.hpp"

namespace Cobalt::Engine
{
    struct Camera
    {
        Vec2 position = {};
        f32 rotation = 0.0f;
        f32 size = 5.0f;
        Color clear_color = Color::from_scalar(0.0f);

        auto view() -> Mat4;
        auto projection(const Vec<2, i32>& viewport_size) const -> Mat4;
        auto view_projection(const Vec<2, i32>& viewport_size) -> Mat4;
    };
}

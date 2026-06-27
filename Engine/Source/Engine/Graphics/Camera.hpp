// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Color.hpp"
#include "Engine/Core/Types/Math.hpp"

namespace Cobalt
{
    struct Camera
    {
        Vec2 position = {};
        f32 rotation = 0.0f;
        f32 size = 5.0f;
        Color clear_color = Color::FromScalar(0.0f);

        auto GetView() -> Mat4;
        auto GetProjection(const Vec<2, i32>& viewport_size) const -> Mat4;
        auto GetViewProjection(const Vec<2, i32>& viewport_size) -> Mat4;
    };
} // namespace Cobalt

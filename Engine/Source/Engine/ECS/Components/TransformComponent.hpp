// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"

namespace Cobalt
{
    struct TransformComponent
    {
        Vec2 position = {0.0f, 0.0f};
        Vec2 scale = {1.0f, 1.0f};
        f32 rotation = 0.0f;

        auto get_transform_matrix() -> Mat4 {
            return glm::translate(Mat4(1), {position.x, position.y, 0}) *
                    glm::rotate(Mat4(1), glm::radians(rotation), {0, 0, 1}) *
                    glm::scale(Mat4(1), {scale.x, scale.y, 1});
        }
    };
} // namespace Cobalt

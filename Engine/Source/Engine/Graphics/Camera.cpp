// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Camera.hpp"

namespace Cobalt::Engine
{
    auto Camera::view() -> Mat4 {
        auto transform = Mat4(1.0f);
        transform = glm::translate(transform, {position.x, position.y, 0.0f});
        transform = glm::rotate(transform, glm::radians(rotation), {0, 0, 1});

        return glm::inverse(transform);
    }
    
    auto Camera::projection(const Vec<2, i32>& viewport_size) const -> Mat4 {
        const f32 aspect_ratio = static_cast<f32>(viewport_size.x) / static_cast<f32>(viewport_size.y);
        const auto left = -(aspect_ratio / 2.0f) * size;
        const auto right = (aspect_ratio / 2.0f) * size;
        const auto bottom = -size / 2.0f;
        const auto top = size / 2.0f;
        
        return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    }

    auto Camera::view_projection(const Vec<2, i32>& viewport_size) -> Mat4 {
        const auto v = view();
        const auto p = projection(viewport_size); 
        return p * v;
    }
}

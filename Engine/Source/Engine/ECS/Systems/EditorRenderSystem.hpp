// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"
#include "Engine/Graphics/Fwd.hpp"

namespace Cobalt::Engine
{
    class EditorRenderSystem final : public ISystem
    {
    public:
        EditorRenderSystem(Renderer* renderer, Camera* camera, Framebuffer* framebuffer);
        auto update(entt::registry& registry) -> void override;

    private:
        Renderer* m_renderer = nullptr;
        Camera* m_camera = nullptr;
        Framebuffer* m_framebuffer = nullptr;
    };
} // namespace Cobalt::Engine


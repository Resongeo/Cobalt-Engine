// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"
#include "Engine/Graphics/Fwd.hpp"

namespace Cobalt
{
    class EditorRenderSystem final : public ISystem
    {
    public:
        EditorRenderSystem(Renderer* renderer, Camera* camera, Framebuffer* framebuffer);
        auto Update(EngineContext& ctx, entt::registry& registry) -> void override;

    private:
        Renderer* _renderer = nullptr;
        Camera* _camera = nullptr;
        Framebuffer* _framebuffer = nullptr;
    };
} // namespace Cobalt

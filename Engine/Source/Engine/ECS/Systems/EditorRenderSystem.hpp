// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"
#include "Engine/Graphics/Fwd.hpp"
#include "Engine/Core/Fwd.hpp"

namespace Cobalt::Engine
{
    class EditorRenderSystem final : public ISystem
    {
    public:
        EditorRenderSystem(Renderer* renderer, Window* window, Camera* camera);
        auto update(entt::registry& registry) -> void override;

    private:
        Renderer* m_renderer = nullptr;
        Window* m_window = nullptr;
        Camera* m_camera = nullptr;
    };
}
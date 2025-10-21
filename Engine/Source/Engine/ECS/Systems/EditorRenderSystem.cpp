// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Core/Application.hpp"
#include "Engine/Graphics/Renderer.hpp"

namespace Cobalt::Engine
{
    EditorRenderSystem::EditorRenderSystem(Renderer* renderer, Camera* camera)
        : m_renderer(renderer), m_camera(camera) { }

    auto EditorRenderSystem::update(entt::registry& registry) -> void {
        const auto& window = Application::get_window();

        const auto viewport_size = window.size();
        m_renderer->set_viewport_size(viewport_size);
        m_renderer->begin_frame(*m_camera);

        for (const auto entity : registry.view<SpriteComponent>()) {
            auto [tint] = registry.get<SpriteComponent>(entity);
            auto [pos, scale] = registry.get<TransformComponent>(entity);
            m_renderer->submit_quad({pos.x, pos.y, 0}, scale, tint);
        }

        m_renderer->end_frame();
    }
}

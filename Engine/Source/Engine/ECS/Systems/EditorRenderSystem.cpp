// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Graphics/Renderer.hpp"

namespace Cobalt::Engine
{
    EditorRenderSystem::EditorRenderSystem(Renderer* renderer, Camera* camera, Framebuffer* framebuffer)
        : m_renderer(renderer), m_camera(camera), m_framebuffer(framebuffer) { }

    auto EditorRenderSystem::update(entt::registry& registry) -> void {
        m_framebuffer->bind();
        
        const auto viewport_size = m_framebuffer->get_size();
        m_renderer->set_viewport_size(viewport_size);
        m_renderer->begin_frame(*m_camera);

        for (const auto entity : registry.view<SpriteComponent>()) {
            auto [tint] = registry.get<SpriteComponent>(entity);
            auto [pos, scale, rotation] = registry.get<TransformComponent>(entity);
            if (rotation == 0) {
                m_renderer->submit_quad({pos.x, pos.y, 0}, scale, tint);    
            } else {
                m_renderer->submit_quad({pos.x, pos.y, 0}, scale, rotation, tint);
            }
            
        }

        m_renderer->end_frame();
        m_framebuffer->unbind();
    }
}

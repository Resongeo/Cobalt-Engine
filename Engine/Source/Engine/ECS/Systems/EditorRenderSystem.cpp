// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Graphics/Renderer.hpp"

namespace Cobalt
{
    EditorRenderSystem::EditorRenderSystem(Renderer* renderer, Camera* camera, Framebuffer* framebuffer) :
        _renderer(renderer), _camera(camera), _framebuffer(framebuffer) {}

    auto EditorRenderSystem::Update(EngineContext& ctx, entt::registry& registry) -> void {
        _framebuffer->Bind();

        const auto viewport_size = _framebuffer->GetSize();
        _renderer->SetViewportSize(viewport_size);
        _renderer->BeginFrame(*_camera);

        for (const auto entity : registry.view<SpriteComponent>()) {
            auto [tint, texture_id] = registry.get<SpriteComponent>(entity);
            auto [pos, scale, rotation] = registry.get<TransformComponent>(entity);

            if (auto texture = AssetManager::Get().GetAsset<Texture2D>(ctx, texture_id); texture) {
                _renderer->SubmitQuad({pos.x, pos.y, 0}, scale, rotation, tint, texture);
            } else {
                _renderer->SubmitQuad({pos.x, pos.y, 0}, scale, rotation, tint);
            }
        }

        _renderer->EndFrame();
        _framebuffer->Unbind();
    }
} // namespace Cobalt

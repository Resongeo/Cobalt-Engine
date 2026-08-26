// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"

namespace Cobalt
{
    auto EditorRenderSystem::Update(entt::registry& registry) -> void {
        FRAME_PROFILER_EVENT("Editor Render System");

        Framebuffer* framebuffer = nullptr;
        Camera* camera = nullptr;
        Renderer* renderer = nullptr;

        if (!registry.ctx().find<Framebuffer*>()) return;
        if (!registry.ctx().find<Camera*>()) return;
        if (!registry.ctx().find<Renderer*>()) return;

        framebuffer = registry.ctx().get<Framebuffer*>();
        camera = registry.ctx().get<Camera*>();
        renderer = registry.ctx().get<Renderer*>();

        framebuffer->Bind();

        const auto viewport_size = framebuffer->GetSize();
        renderer->SetViewportSize(viewport_size);
        renderer->BeginFrame(*camera);

        for (const auto entity : registry.view<SpriteComponent>()) {
            auto [tint, texture_id] = registry.get<SpriteComponent>(entity);
            auto [pos, scale, rotation] = registry.get<TransformComponent>(entity);

            if (auto texture = AssetManager::Get().GetAsset<Texture2D>(texture_id); texture) {
                renderer->SubmitQuad({pos.x, pos.y, 0}, scale, rotation, tint, texture);
            } else {
                renderer->SubmitQuad({pos.x, pos.y, 0}, scale, rotation, tint);
            }
        }

        renderer->EndFrame();
        framebuffer->Unbind();
    }
} // namespace Cobalt

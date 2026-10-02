// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/EditorUpdateSystem.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/ECS/Components/SpriteComponent.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Graphics/RenderTarget.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"

#include <optick.h>

namespace Cobalt
{
    auto EditorUpdateSystem::Update(entt::registry& registry) -> void {
        OPTICK_EVENT();
        FRAME_PROFILER_EVENT("Editor Render System");

        if (!registry.ctx().find<Camera*>()) return;
        if (!registry.ctx().find<RenderTarget*>()) return;

        const auto* camera = registry.ctx().get<Camera*>();
        const auto* render_target = registry.ctx().get<RenderTarget*>();
        if (!render_target || !render_target->texture) return;

        HashMap<SDL_GPUTexture*, std::vector<SpriteInstance>> texture_batches;

        const auto view = registry.view<TransformComponent, SpriteComponent>();
        for (const auto entity : view) {
            const auto& [pos, scale, rotation] = registry.get<TransformComponent>(entity);
            const auto& [tint, texture_uuid] = registry.get<SpriteComponent>(entity);

            SDL_GPUTexture* gpu_texture = nullptr;
            if (const auto texture = AssetManager::Get().GetAsset<Texture2D>(texture_uuid)) {
                gpu_texture = texture->GetGPUTexture();
            }
            if (!gpu_texture) continue;

            SpriteInstance instance = {.x = pos.x,
                                       .y = pos.y,
                                       .z = 0.0f,
                                       .rotation = glm::radians(rotation),
                                       .w = scale.x,
                                       .h = scale.y,
                                       .padding_a = 0.0f,
                                       .padding_b = 0.0f,
                                       .tex_u = 0.0f,
                                       .tex_v = 0.0f,
                                       .tex_w = 1.0f,
                                       .tex_h = 1.0f,
                                       .r = tint.r,
                                       .g = tint.g,
                                       .b = tint.b,
                                       .a = tint.a};

            texture_batches[gpu_texture].push_back(instance);
        }

        Vector<SpriteBatch> batches;
        batches.reserve(texture_batches.size());
        for (auto& [texture, sprites] : texture_batches) {
            batches.push_back({texture, std::move(sprites)});
        }

        RenderPass pass;
        pass.render_target = render_target->texture;
        pass.width = render_target->width;
        pass.height = render_target->height;
        pass.camera = camera;
        pass.clear = true;
        pass.batches = eastl::move(batches);

        Renderer::SubmitPass(pass);
    }
} // namespace Cobalt

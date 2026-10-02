// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/RuntimeUpdateSystem.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Graphics/RenderTarget.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"
#include "Engine/Scripting/Script.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

namespace Cobalt
{
    void RuntimeUpdateSystem::Update(entt::registry& registry) {
        FRAME_PROFILER_EVENT("Script Update");

        for (const auto entity : registry.view<ScriptComponent>()) {
            auto& [script_id, instance] = registry.get<ScriptComponent>(entity);

            if (!script_id.IsValid()) {
                continue;
            }

            if (auto script = AssetManager::Get().GetAsset<Script>(script_id); script) {
                ScriptManager::Get().ExecuteUpdate(script, instance, Time::GetDeltaTime());
            }
        }

        Camera* primary_camera = nullptr;
        bool first_camera = true;
        for (const auto entity : registry.view<CameraComponent>()) {
            const auto transform = registry.get<TransformComponent>(entity);
            auto& [camera, primary] = registry.get<CameraComponent>(entity);

            camera.position = transform.position;
            camera.rotation = transform.rotation;

            if (first_camera) {
                primary_camera = &camera;
                first_camera = false;
            }

            if (primary) {
                primary_camera = &camera;
            }
        }

        if (primary_camera == nullptr) {
            return;
        }

        if (!registry.ctx().find<RenderTarget*>()) return;
        const auto* render_target = registry.ctx().get<RenderTarget*>();

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
        pass.camera = primary_camera;
        pass.clear = true;
        pass.batches = eastl::move(batches);

        Renderer::SubmitPass(pass);
    }
} // namespace Cobalt

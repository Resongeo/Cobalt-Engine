// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/RuntimeUpdateSystem.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Graphics/Renderer.hpp"
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

        Framebuffer* framebuffer = nullptr;
        Renderer* renderer = nullptr;

        if (!registry.ctx().find<Framebuffer*>()) return;
        if (!registry.ctx().find<Renderer*>()) return;

        framebuffer = registry.ctx().get<Framebuffer*>();
        renderer = registry.ctx().get<Renderer*>();

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

        framebuffer->Bind();

        const auto viewport_size = framebuffer->GetSize();
        renderer->SetViewportSize(viewport_size);
        renderer->BeginFrame(*primary_camera);

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

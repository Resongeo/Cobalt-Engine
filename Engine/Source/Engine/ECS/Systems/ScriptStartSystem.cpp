// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/ScriptStartSystem.hpp"
#include "Engine/ECS/Components/ScriptComponent.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Scripting/Script.hpp"

namespace Cobalt
{
    void ScriptStartSystem::Update(EngineContext& ctx, entt::registry& registry) {
        for (const auto entity : registry.view<ScriptComponent>()) {
            auto& [script_id, instance] = registry.get<ScriptComponent>(entity);

            if (!script_id.IsValid()) {
                continue;
            }

            if (auto script = AssetManager::Get().GetAsset<Script>(ctx, script_id); script) {
                ctx.script_manager.CompileScript(script);
                instance = ctx.script_manager.InstantiateScript(ctx, entity, script);
                ctx.script_manager.ExecuteStart(script, instance);
            }
        }
    }
} // namespace Cobalt

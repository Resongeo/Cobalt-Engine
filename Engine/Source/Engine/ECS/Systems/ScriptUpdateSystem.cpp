// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/ScriptUpdateSystem.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Scripting/Script.hpp"

namespace Cobalt
{
    void ScriptUpdateSystem::update(EngineContext& ctx, entt::registry& registry) {
        for (const auto entity : registry.view<ScriptComponent>()) {
            auto& [script_id, instance] = registry.get<ScriptComponent>(entity);

            if (!script_id.is_valid()) {
                continue;
            }

            if (auto script = ctx.asset_manager.get_asset<Script>(ctx, script_id); script) {
                ctx.script_manager.execute_update(script, instance, ctx.delta_time);
            }
        }
    }
} // namespace Cobalt

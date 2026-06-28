// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/ScriptUpdateSystem.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Scripting/Script.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

namespace Cobalt
{
    void ScriptUpdateSystem::Update(entt::registry& registry) {
        for (const auto entity : registry.view<ScriptComponent>()) {
            auto& [script_id, instance] = registry.get<ScriptComponent>(entity);

            if (!script_id.IsValid()) {
                continue;
            }

            if (auto script = AssetManager::Get().GetAsset<Script>(script_id); script) {
                ScriptManager::Get().ExecuteUpdate(script, instance, Time::GetDeltaTime());
            }
        }
    }
} // namespace Cobalt

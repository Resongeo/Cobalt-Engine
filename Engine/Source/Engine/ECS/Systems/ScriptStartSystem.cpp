// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/ECS/Systems/ScriptStartSystem.hpp"
#include "Engine/ECS/Components/ScriptComponent.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Scripting/Script.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

namespace Cobalt
{
    void ScriptStartSystem::Update(entt::registry& registry) {
        for (const auto entity : registry.view<ScriptComponent>()) {
            auto& [script_id, instance] = registry.get<ScriptComponent>(entity);

            if (!script_id.IsValid()) {
                continue;
            }

            if (auto script = AssetManager::Get().GetAsset<Script>(script_id); script) {
                auto& script_manager = ScriptManager::Get();
                script_manager.CompileScript(script);
                instance = script_manager.InstantiateScript(entity, script);
                script_manager.ExecuteStart(script, instance);
            }
        }
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"

namespace Cobalt
{
    class ScriptStartSystem final : public ISystem
    {
    public:
        ScriptStartSystem() = default;
        auto update(EngineContext& ctx, entt::registry& registry) -> void override;
    };
} // namespace Cobalt

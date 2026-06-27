// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"

namespace Cobalt
{
    class ScriptUpdateSystem final : public ISystem
    {
    public:
        ScriptUpdateSystem() = default;
        auto Update(EngineContext& ctx, entt::registry& registry) -> void override;
    };
} // namespace Cobalt

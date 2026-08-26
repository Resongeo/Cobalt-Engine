// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"

namespace Cobalt
{
    class EditorUpdateSystem final : public ISystem
    {
    public:
        explicit EditorUpdateSystem() = default;
        auto Update(entt::registry& registry) -> void override;
    };
} // namespace Cobalt

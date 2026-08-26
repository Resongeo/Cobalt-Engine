// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"

namespace Cobalt
{
    class EditorRenderSystem final : public ISystem
    {
    public:
        explicit EditorRenderSystem() = default;
        auto Update(entt::registry& registry) -> void override;
    };
} // namespace Cobalt

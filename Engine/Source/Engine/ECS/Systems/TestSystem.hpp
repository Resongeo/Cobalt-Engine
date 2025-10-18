// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/ECS/Systems/ISystem.hpp"

namespace Cobalt::Engine
{
    class TestSystem final : public ISystem
    {
    public:
        auto update(entt::registry& registry) -> void override;
    };
}
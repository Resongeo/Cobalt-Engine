// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <entt/entity/registry.hpp>

namespace Cobalt::Engine
{
    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        virtual auto update(entt::registry& registry) -> void {}
    };
}
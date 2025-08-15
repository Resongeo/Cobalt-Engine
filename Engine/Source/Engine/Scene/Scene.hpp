// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types.hpp"
#include <entt/entity/registry.hpp>

namespace Cobalt::Engine
{
    class Scene final
    {
    public:
        explicit Scene() = default;
        explicit Scene(const String& name);

        auto name() -> String&;
        auto registry() -> entt::registry&;

    private:
        String m_name = {};
        entt::registry m_registry = {};
    };
}
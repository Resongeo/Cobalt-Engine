// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"
#include "Engine/ECS/Entity.hpp"

namespace Cobalt::Engine
{
    class Scene final
    {
    public:
        explicit Scene() = default;
        explicit Scene(const String& name);

        auto name() -> String&;
        auto registry() -> entt::registry&;
        auto create_entity(const String& name) -> Entity;
        auto create_entity(const String& name, const UUID uuid) -> Entity;
        auto create_empty_entity() -> Entity;

    private:
        String m_name = {};
        entt::registry m_registry = {};
    };
} // namespace Cobalt::Engine


// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"
#include "Engine/ECS/Entity.hpp"
#include "Engine/Assets/IAsset.hpp"


namespace Cobalt
{
    class Scene final : public IAsset
    {
    public:
        explicit Scene() = default;
        explicit Scene(const String& name);

        auto get_name() -> String&;
        auto set_name(const String& name) -> void;
        auto get_registry() -> entt::registry&;
        auto create_entity(const String& name) -> Entity;
        auto create_entity(const String& name, UUID uuid) -> Entity;
        auto create_empty_entity() -> Entity;

    private:
        String m_name = {};
        entt::registry m_registry = {};
    };
} // namespace Cobalt

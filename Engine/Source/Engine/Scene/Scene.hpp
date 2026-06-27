// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Memory.hpp"
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

        auto GetName() -> String&;
        auto SetName(const String& name) -> void;
        auto GetRegistry() -> entt::registry&;
        auto CreateEntity(const String& name) -> Entity;
        auto CreateEntity(const String& name, UUID uuid) -> Entity;
        auto CreateEmptyEntity() -> Entity;
        auto Clone() -> Rc<Scene>;

    private:
        String _name = {};
        entt::registry _registry = {};
    };
} // namespace Cobalt

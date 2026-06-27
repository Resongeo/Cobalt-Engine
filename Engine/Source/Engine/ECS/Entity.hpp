// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <entt/entt.hpp>

namespace Cobalt
{
    class Entity final
    {
    public:
        Entity(const entt::entity id, entt::registry* registry) : _id(id), _registry(registry) {}
        Entity() = default;

        auto IsValid() const -> bool {
            return _registry || _id != entt::null;
        }

        template <typename T, typename... Args>
        auto AddComponent(Args&&... args) -> T& {
            return _registry->emplace<T>(_id, std::forward<Args>(args)...);
        }

        template <typename T>
        auto HasComponent() const -> bool {
            return _registry->any_of<T>(_id);
        }

        template <typename T>
        auto GetComponent() -> T& {
            return _registry->get<T>(_id);
        }

        template <typename T>
        auto RemoveComponent() const -> void {
            _registry->remove<T>(_id);
        }

        auto ID() const -> entt::entity {
            return _id;
        }

    private:
        entt::entity _id = entt::null;
        entt::registry* _registry = nullptr;
    };
} // namespace Cobalt

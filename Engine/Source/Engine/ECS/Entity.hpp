// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <entt/entt.hpp>

namespace Cobalt::Engine
{
    class Entity final
    {
    public:
        Entity(const entt::entity id, entt::registry* registry)
            : m_id(id), m_registry(registry) {}

        template<typename T, typename... Args>
        auto add_component(Args&&... args) -> void {
            m_registry->emplace<T>(m_id, std::forward<Args>(args)...);
        }
        
        template<typename T>
        auto has_component() const -> bool {
            return m_registry->any_of<T>(m_id);
        }
        
        template<typename T>
        auto get_component() -> T& {
            return m_registry->get<T>(m_id);
        }
        
        template<typename T>
        auto remove_component() const -> void {
            m_registry->remove<T>(m_id);
        }

        auto get_id() const -> entt::entity {
            return m_id;
        }

    private:
        entt::entity m_id = entt::null;
        entt::registry* m_registry = nullptr;
    };
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <entt/signal/dispatcher.hpp>

namespace Cobalt
{
    class EventBus final
    {
    public:
        template<typename Event, typename... Args>
        static void Trigger(Args&&... args) {
            _dispatcher.trigger<Event>(std::forward<Args>(args)...);
        }

        template<typename Event>
        static void Trigger(Event&& event) {
            _dispatcher.trigger(std::forward<Event>(event));
        }

        template<typename Event, auto Member, typename Instance>
        static void Subscribe(Instance* instance) {
            _dispatcher.sink<Event>().template connect<Member>(instance);
        }

        template<typename Event, auto Member, typename Instance>
        static void Unsubscribe(Instance* instance) {
            _dispatcher.sink<Event>().template disconnect<Member>(instance);
        }

    private:
        inline static entt::dispatcher _dispatcher = {};
    };
}
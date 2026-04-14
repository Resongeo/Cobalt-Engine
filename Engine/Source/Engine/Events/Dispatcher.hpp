// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Events/IEvent.hpp"

#include <functional>

namespace Cobalt
{
    class EventDispatcher final
    {
    public:
        template <typename T>
        auto dispatch(const std::function<void(T&)>& callback, IEvent& event) -> void {
            if (event.type() == T::static_type()) {
                callback(*static_cast<T*>(&event));
            }
        }
    };

#define BIND(callback) std::bind(&callback, this, std::placeholders::_1)
} // namespace Cobalt

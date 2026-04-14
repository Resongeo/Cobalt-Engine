// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Events/EventType.hpp"

namespace Cobalt
{
    class IEvent
    {
    public:
        virtual ~IEvent() = default;

        virtual auto type() const -> EventType = 0;
        virtual auto name() const -> const char* = 0;
    };

#define EVENT_BUILDER(event_type)                                                                                      \
    static auto static_type() -> EventType {                                                                           \
        return event_type;                                                                                             \
    }                                                                                                                  \
    auto type() const -> EventType override {                                                                          \
        return static_type();                                                                                          \
    }                                                                                                                  \
    auto name() const -> const char* override {                                                                        \
        return #event_type;                                                                                            \
    }
} // namespace Cobalt

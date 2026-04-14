// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Events/IEvent.hpp"

namespace Cobalt
{
    class GamepadAxisMotionEvent : public IEvent
    {
    public:
        explicit GamepadAxisMotionEvent() = default;
        EVENT_BUILDER(EventType::GamepadAxisMotion)
        // TODO: Implement GamepadAxisMotionEvent properly when working on Input
    };

    class GamepadButtonDownEvent : public IEvent
    {
    public:
        explicit GamepadButtonDownEvent() = default;
        EVENT_BUILDER(EventType::GamepadButtonDown)
        // TODO: Implement GamepadButtonDownEvent properly when working on Input
    };

    class GamepadButtonUpEvent : public IEvent
    {
    public:
        explicit GamepadButtonUpEvent() = default;
        EVENT_BUILDER(EventType::GamepadButtonUp)
        // TODO: Implement GamepadButtonUpEvent properly when working on Input
    };

    class GamepadAddedEvent : public IEvent
    {
    public:
        explicit GamepadAddedEvent() = default;
        EVENT_BUILDER(EventType::GamepadAdded)
        // TODO: Implement GamepadAddedEvent properly when working on Input
    };

    class GamepadRemovedEvent : public IEvent
    {
    public:
        explicit GamepadRemovedEvent() = default;
        EVENT_BUILDER(EventType::GamepadRemoved)
        // TODO: Implement GamepadRemovedEvent properly when working on Input
    };
} // namespace Cobalt

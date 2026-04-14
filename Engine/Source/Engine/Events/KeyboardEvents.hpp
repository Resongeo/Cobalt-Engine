// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Events/IEvent.hpp"

namespace Cobalt
{
    class KeyboardKeyDownEvent : public IEvent
    {
    public:
        explicit KeyboardKeyDownEvent() = default;
        EVENT_BUILDER(EventType::KeyboardKeyDown)
        // TODO: Implement KeyboardKeyDownEvent properly when working on Input
    };

    class KeyboardKeyUpEvent : public IEvent
    {
    public:
        explicit KeyboardKeyUpEvent() = default;
        EVENT_BUILDER(EventType::KeyboardKeyUp)
        // TODO: Implement KeyboardKeyUpEvent properly when working on Input
    };

    class KeyboardTextInputEvent : public IEvent
    {
    public:
        explicit KeyboardTextInputEvent() = default;
        EVENT_BUILDER(EventType::KeyboardTextInput)
        // TODO: Implement KeyboardTextInputEvent properly when working on Input
    };
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Events/IEvent.hpp"

namespace Cobalt::Engine
{
    class DropFileEvent : public IEvent
    {
    public:
        explicit DropFileEvent(const Filepath& filename) : filename(filename) {}
        Filepath filename = {};

        EVENT_BUILDER(EventType::DropFile)
    };

    class DropTextEvent : public IEvent
    {
    public:
        explicit DropTextEvent(const String& text) : text(text) {}
        String text = {};

        EVENT_BUILDER(EventType::DropText)
    };

    class DropBeginEvent : public IEvent
    {
    public:
        explicit DropBeginEvent() = default;
        EVENT_BUILDER(EventType::DropBegin)
    };

    class DropCompleteEvent : public IEvent
    {
    public:
        explicit DropCompleteEvent() = default;
        EVENT_BUILDER(EventType::DropComplete)
    };

    class DropPositionEvent : public IEvent
    {
    public:
        explicit DropPositionEvent(const u32 x, const u32 y) : x(x), y(y) {}
        u32 x = 0;
        u32 y = 0;
        
        EVENT_BUILDER(EventType::DropPosition)
    };
} // namespace Cobalt::Engine

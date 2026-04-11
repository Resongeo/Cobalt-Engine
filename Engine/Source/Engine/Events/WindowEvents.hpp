// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Events/IEvent.hpp"

namespace Cobalt::Engine
{
    class WindowMovedEvent : public IEvent
    {
    public:
        explicit WindowMovedEvent(const u32 x, const u32 y) : x(x), y(y) {}
        u32 x = 0;
        u32 y = 0;

        EVENT_BUILDER(EventType::WindowMoved)
    };

    class WindowResizedEvent : public IEvent
    {
    public:
        explicit WindowResizedEvent(const u32 width, const u32 height) : width(width), height(height) {}
        u32 width = 0;
        u32 height = 0;

        EVENT_BUILDER(EventType::WindowResized)
    };

    class WindowMinimizedEvent : public IEvent
    {
    public:
        explicit WindowMinimizedEvent() = default;
        EVENT_BUILDER(EventType::WindowMinimized)
    };

    class WindowMaximizedEvent : public IEvent
    {
    public:
        explicit WindowMaximizedEvent() = default;
        EVENT_BUILDER(EventType::WindowMaximized)
    };

    class WindowRestoredEvent : public IEvent
    {
    public:
        explicit WindowRestoredEvent() = default;
        EVENT_BUILDER(EventType::WindowRestored)
    };

    class WindowEnterFullscreenEvent : public IEvent
    {
    public:
        explicit WindowEnterFullscreenEvent() = default;
        EVENT_BUILDER(EventType::WindowEnterFullscreen)
    };

    class WindowLeaveFullscreenEvent : public IEvent
    {
    public:
        explicit WindowLeaveFullscreenEvent() = default;
        EVENT_BUILDER(EventType::WindowLeaveFullscreen)
    };
} // namespace Cobalt::Engine

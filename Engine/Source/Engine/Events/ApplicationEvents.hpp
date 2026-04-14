// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Events/IEvent.hpp"

namespace Cobalt
{
    class ApplicationQuitEvent final : public IEvent
    {
    public:
        explicit ApplicationQuitEvent() = default;
        EVENT_BUILDER(EventType::ApplicationQuit)
    };
} // namespace Cobalt

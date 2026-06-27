// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"

namespace Cobalt
{
    struct WindowMovedEvent
    {
        u32 x = 0;
        u32 y = 0;
    };

    struct WindowResizedEvent
    {
        u32 width = 0;
        u32 height = 0;
    };

    struct WindowMinimizedEvent {};

    struct WindowMaximizedEvent {};

    struct WindowRestoredEvent {};

    struct WindowEnterFullscreenEvent {};

    struct WindowLeaveFullscreenEvent {};
} // namespace Cobalt

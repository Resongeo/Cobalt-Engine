// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt
{
    struct DropFileEvent
    {
        Filepath filename = {};
    };

    struct DropTextEvent
    {
        String text = {};
    };

    struct DropPositionEvent
    {
        u32 x = 0;
        u32 y = 0;
    };

    struct DropBeginEvent {};

    struct DropCompleteEvent {};
} // namespace Cobalt

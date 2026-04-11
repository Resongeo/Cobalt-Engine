// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"

namespace Cobalt::Engine
{
    enum class EventType : u8
    {
        None = 0,

        ApplicationQuit,

        WindowMoved,
        WindowResized,
        WindowMinimized,
        WindowMaximized,
        WindowRestored,
        WindowEnterFullscreen,
        WindowLeaveFullscreen,

        KeyboardKeyDown,
        KeyboardKeyUp,
        KeyboardTextInput,

        MouseMotion,
        MouseButtonDown,
        MouseButtonUp,
        MouseWheel,

        GamepadAxisMotion,
        GamepadButtonDown,
        GamepadButtonUp,
        GamepadAdded,
        GamepadRemoved,

        DropFile,
        DropText,
        DropBegin,
        DropComplete,
        DropPosition,
    };
}

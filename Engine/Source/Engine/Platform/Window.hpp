// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Error.hpp"
#include "Engine/Core/Types/Math.hpp"

struct SDL_Window;

namespace Cobalt
{
    enum class WindowInitError
    {
        SDLInit,
        WindowCreate,
    };

    using NativeEventCallback = std::function<void(void*)>;

    namespace Window
    {
        auto Initialize() -> Result<void, WindowInitError>;
        auto Shutdown() -> void;
        auto PollEvents() -> void;

        auto GetHandle() -> SDL_Window*;
        auto GetSize() -> Vec<2, i32>;

        auto SetTitle(const String& title) -> void;
        auto SetNativeEventCallback(NativeEventCallback callback) -> void;
    }; // namespace Window
} // namespace Cobalt

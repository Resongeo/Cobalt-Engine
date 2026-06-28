// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Fwd.hpp"
#include "Engine/Core/Types/Math.hpp"
#include "Engine/Platform/Fwd.hpp"

namespace Cobalt
{
    using NativeEventCallback = std::function<void(void*)>;

    class Window final
    {
    public:
        auto Init() -> bool;

        auto PollEvents(EngineContext& ctx) const -> void;
        auto SwapBuffers() const -> void;
        auto ShutDown() const -> void;

        auto SetNativeEventCallback(NativeEventCallback callback) -> void;
        auto GetHandle() const -> SDL_Window*;
        auto GetGLContext() const -> SDL_GLContextState*;
        auto GetSize() const -> Vec<2, i32>;

        static auto Get() -> Window&;

    private:
        SDL_Window* _handle = nullptr;
        NativeEventCallback _native_event_hook = nullptr;
    };
} // namespace Cobalt

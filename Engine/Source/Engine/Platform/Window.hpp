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
        auto init(EngineContext& ctx) -> bool;
        auto poll_events(EngineContext& ctx) const -> void;
        auto swap_buffers() const -> void;
        auto destroy() const -> void;

        auto set_native_event_callback(NativeEventCallback callback) -> void;
        auto get_handle() const -> SDL_Window*;
        auto get_gl_context() const -> SDL_GLContextState*;
        auto get_size() const -> Vec<2, i32>;

    private:
        SDL_Window* m_handle = nullptr;
        NativeEventCallback m_native_event_hook = nullptr;
    };
} // namespace Cobalt

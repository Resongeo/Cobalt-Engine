// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"

struct SDL_Window;
struct SDL_GLContextState;

namespace Cobalt::Engine
{
    class Window final
    {
    public:
        static auto initialize() -> void;
        static auto instance() -> Window&;
        static auto destroy() -> void;

        auto swap_buffers() const -> void;

        auto get_handle() const -> SDL_Window*;
        auto get_gl_context() const -> SDL_GLContextState*;
        auto get_size() const -> Vec<2, i32>;

    private:
        auto _create() -> bool;

    private:
        SDL_Window* m_handle = nullptr;
    };
}

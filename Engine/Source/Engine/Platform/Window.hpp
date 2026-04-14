// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Project.hpp"
#include "Engine/Core/Types/Math.hpp"
#include "Engine/Platform/Fwd.hpp"

namespace Cobalt
{
    class Window final
    {
    public:
        auto init(Project& project) -> bool;
        auto swap_buffers() const -> void;
        auto destroy() const -> void;

        auto get_handle() const -> SDL_Window*;
        auto get_gl_context() const -> SDL_GLContextState*;
        auto get_size() const -> Vec<2, i32>;

    private:
        SDL_Window* m_handle = nullptr;
    };
} // namespace Cobalt

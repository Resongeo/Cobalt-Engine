// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"

struct GLFWwindow;

namespace Cobalt::Engine
{
    class Window final
    {
    public:
        auto create() -> void;
        auto close_requested() const -> bool;
        auto poll_events() const -> void;
        auto swap_buffers() const -> void;
        auto destroy() const -> void;
        auto handle() const -> GLFWwindow*;
        auto size() const -> Vec<2, i32>;

    private:
        GLFWwindow* m_handle = nullptr;
    };
}

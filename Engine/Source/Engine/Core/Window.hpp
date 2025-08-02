// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include <GLFW/glfw3.h>

namespace Cobalt::Engine
{
    class Window final
    {
    public:
        auto create() -> void;
        auto close_requested() const -> bool;
        auto swap_buffers() const -> void;
        auto destroy() const -> void;

    private:
        GLFWwindow* m_handle = nullptr;
    };
}

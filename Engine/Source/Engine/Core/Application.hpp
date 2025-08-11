// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Window.hpp"
#include "Engine/Scene/SceneManager.hpp"

namespace Cobalt::Engine
{
    class Application
    {
    public:
        virtual ~Application() = default;

        auto run() -> void;

        virtual auto on_begin() -> void {}
        virtual auto on_update() -> void {}
        virtual auto on_end() -> void {}

    private:
        auto initialize() -> void;
        auto main_loop() -> void;
        auto cleanup() const -> void;

    protected:
        Window p_window;
        SceneManager p_scene_manager;
    };
}

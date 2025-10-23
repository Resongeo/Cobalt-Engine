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
        Application();

        auto run() -> void;

        virtual auto on_begin() -> void {}
        virtual auto on_update() -> void {}
        virtual auto on_end() -> void {}

        static auto get_window() -> Window&;
        static auto get_scene_manager() -> SceneManager&;

    private:
        auto _initialize() -> void;
        auto _main_loop() -> void;
        auto _cleanup() const -> void;

    private:
        Window m_window;
        SceneManager m_scene_manager;
    };
}

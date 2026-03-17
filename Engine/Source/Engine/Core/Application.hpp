// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Scene/SceneManager.hpp"

union SDL_Event;

namespace Cobalt::Engine
{
    class Application
    {
    public:
        virtual ~Application() = default;

        auto run() -> void;

        virtual auto on_begin() -> void {}
        virtual auto on_event(SDL_Event* event) -> void {}
        virtual auto on_update() -> void {}
        virtual auto on_end() -> void {}

    private:
        auto _initialize() const -> void;
        auto _main_loop() -> void;
        auto _cleanup() const -> void;

    private:
        SceneManager m_scene_manager;
        bool m_close_requested = false;
    };
}

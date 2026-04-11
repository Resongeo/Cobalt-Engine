// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Events/ApplicationEvents.hpp"
#include "Engine/Events/IEvent.hpp"
#include "Engine/Scene/SceneManager.hpp"

union SDL_Event;

namespace Cobalt::Engine
{
    class Application
    {
    public:
        virtual ~Application();

        auto run() -> void;

        virtual auto on_begin() -> void {}
        virtual auto on_event(IEvent& event) -> void {}
        virtual auto on_update() -> void {}
        virtual auto on_end() -> void {}
        virtual auto on_sdl_event(SDL_Event* event) -> void {}

    private:
        auto _initialize() const -> bool;
        auto _main_loop() -> void;
        auto _cleanup() const -> void;
        auto _poll_events() -> void;

    private:
        SceneManager m_scene_manager;
        bool m_close_requested = false;
    };
} // namespace Cobalt::Engine

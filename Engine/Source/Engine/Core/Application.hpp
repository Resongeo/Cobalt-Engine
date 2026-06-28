// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/CommandLineArgs.hpp"
#include "Engine/Events/ApplicationEvents.hpp"

namespace Cobalt
{
    class Application
    {
    public:
        virtual ~Application();

        auto Run(const CommandLineArgs& args) -> void;

        virtual auto OnBegin() -> void {}
        virtual auto OnUpdate() -> void {}
        virtual auto OnShutdown() -> void {}

    private:
        auto Init(const CommandLineArgs& args) -> bool;
        auto MainLoop() -> void;
        auto OnApplicationQuit(const ApplicationQuitEvent& event) -> void;

    private:
        bool _close_requested = false;
    };
} // namespace Cobalt

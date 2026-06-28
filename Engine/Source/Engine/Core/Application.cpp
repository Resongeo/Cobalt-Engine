// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>

namespace Cobalt
{
    auto Application::Run(const CommandLineArgs& args) -> void {
        if (!Init(args)) {
            CORE_CRITICAL("Application: Initialization failed! Exiting program...");
            return;
        }

        OnBegin(_ctx);
        MainLoop();
        OnShutdown(_ctx);
    }

    auto Application::Init(const CommandLineArgs& args) -> bool {
        Log::Init(_ctx);

        // TODO: Have proper error types and TRY macro
        Project::Get().Init(args);
        AssetManager::Get().Init();
        SceneManager::Get().Init(_ctx);

        if (!Window::Get().Init()) return false;
        if (!ScriptManager::Get().Init(_ctx)) return false;

        DialogManager::Get().Init();
        Time::Init();

        return true;
    }

    auto Application::MainLoop() -> void {
        while (!_ctx.close_requested) {
            Window::Get().PollEvents(_ctx);
            Time::Update();
            Log::FlushEvents(_ctx);
            OnUpdate(_ctx);
            Window::Get().SwapBuffers();
        }
    }

    Application::~Application() {
        AssetManager::Get().SaveRegistry();
        ScriptManager::Get().ShutDown();
        Window::Get().ShutDown();
    }
} // namespace Cobalt

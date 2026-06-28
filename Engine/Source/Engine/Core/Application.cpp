// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Core/Log.hpp"
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

        Project::Get().Init(args);
        /*
        AssetManager::Get().Init();
        SceneManager::Get().Init();
        Window::Get().Init();
        ScriptManager::GetInstance().Init();
        */

        _ctx.asset_manager.Init();
        _ctx.scene_manager.Init(_ctx);

        if (!Window::Get().Init()) return false;
        if (!_ctx.script_manager.Init(_ctx)) return false;

        _ctx.dialog_manager.Init();

        return true;
    }

    auto Application::MainLoop() -> void {
        u64 current_time = SDL_GetTicks();
        u64 last_time = current_time;
        while (!_ctx.close_requested) {
            current_time = SDL_GetTicks();
            _ctx.delta_time = static_cast<f32>(current_time - last_time) / 1000.0f;
            last_time = current_time;

            Log::FlushEvents(_ctx);

            Window::Get().PollEvents(_ctx);
            OnUpdate(_ctx);
            Window::Get().SwapBuffers();
        }
    }

    Application::~Application() {
        _ctx.asset_manager.SaveRegistry();
        _ctx.script_manager.ShutDown();
        Window::Get().ShutDown();
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/JobSystem.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

#include <SDL3/SDL.h>
#include <rpmalloc.h>
#include <optick.h>
#include <optick.config.h>

namespace Cobalt
{
    auto Application::Run(const CommandLineArgs& args) -> void {
        OPTICK_START_CAPTURE();

        if (!Init(args)) {
            CORE_CRITICAL("Application: Initialization failed! Exiting program...");
            return;
        }

        OnBegin();
        MainLoop();
        OnShutdown();

        Shutdown();

        OPTICK_STOP_CAPTURE();
        OPTICK_SAVE_CAPTURE("profile.opt");
        OPTICK_SHUTDOWN();
    }

    auto Application::Init(const CommandLineArgs& args) -> bool {
        OPTICK_EVENT();

        // TODO: Have proper error types and TRY macro
        Memory::Init();
        Log::Init();

        JobSystem::Get().Init();
        Project::Get().Init(args);
        AssetManager::Get().Init();
        SceneManager::Get().Init();

        if (!Window::Get().Init()) return false;
        if (!ScriptManager::Get().Init()) return false;

        DialogManager::Get().Init();
        Time::Init();

        EventBus::Subscribe<ApplicationQuitEvent, &Application::OnApplicationQuit>(this);

        return true;
    }

    auto Application::Shutdown() const -> void {
        AssetManager::Get().Shutdown();
        SceneManager::Get().Shutdown();
        JobSystem::Get().Shutdown();
        ScriptManager::Get().ShutDown();
        Window::Get().ShutDown();
    }

    auto Application::MainLoop() -> void {
        while (!_close_requested) {
            OPTICK_FRAME("MainThread");
            FRAME_PROFILER_BEGIN();

            Window::Get().PollEvents();
            Time::Update();
            Log::FlushEvents();
            OnUpdate();
            OnDraw();
            Window::Get().SwapBuffers();

            FRAME_PROFILER_END();
        }
    }

    auto Application::OnApplicationQuit(const ApplicationQuitEvent& event) -> void {
        _close_requested = true;
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Error.hpp"
#include "Engine/Core/JobSystem.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

#include <SDL3/SDL.h>
#include <optick.config.h>
#include <optick.h>
#include <rpmalloc.h>

namespace Cobalt
{
    auto Application::Run(const CommandLineArgs& args) -> void {
        if (const auto result = Init(args); !result) {
            CORE_CRITICAL("Application: Initialization failed! Error: {}\n Exiting program...",
                          CoreInitErrorStr[static_cast<usize>(result.error())]);
            return;
        }

        OnBegin();
        MainLoop();
        OnShutdown();
        Shutdown();
    }

    auto Application::Init(const CommandLineArgs& args) -> Result<bool, CoreInitError> {
        Memory::Init();
        Log::Init();

        TRY(JobSystem::Get().Init());
        TRY(Project::Get().Init(args));
        TRY(AssetManager::Get().Init());
        TRY(SceneManager::Get().Init());
        TRY(Window::Get().Init());
        TRY(ScriptManager::Get().Init());
        TRY(DialogManager::Get().Init());

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
        OPTICK_START_CAPTURE();

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

        OPTICK_STOP_CAPTURE();
        OPTICK_SAVE_CAPTURE("profile.opt");
        OPTICK_SHUTDOWN();
    }

    auto Application::OnApplicationQuit(const ApplicationQuitEvent& event) -> void {
        _close_requested = true;
    }
} // namespace Cobalt

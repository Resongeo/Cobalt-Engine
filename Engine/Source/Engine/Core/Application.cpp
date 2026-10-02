// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Application.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Error.hpp"
#include "Engine/Core/JobSystem.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"
#include "Engine/Rendering/RHI.hpp"
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
            CORE_CRITICAL("Application: Initialization failed! Error: Exiting program...");
            return;
        }

        OnBegin();
        MainLoop();
        OnShutdown();
        Shutdown();
    }

    auto Application::Init(const CommandLineArgs& args) -> bool {
        Memory::Init();
        Log::Init();

        TRY_CRITICAL(Project::Initialize(args));
        TRY_CRITICAL(Window::Initialize());
        TRY_CRITICAL(RHI::Initialize());
        TRY_CRITICAL(Renderer::Initialize());

        TRY_CRITICAL(JobSystem::Get().Init());
        TRY_CRITICAL(AssetManager::Get().Init());
        TRY_CRITICAL(SceneManager::Get().Init());
        TRY_CRITICAL(ScriptManager::Get().Init());
        TRY_CRITICAL(DialogManager::Get().Init());

        Time::Init();

        EventBus::Subscribe<ApplicationQuitEvent, &Application::OnApplicationQuit>(this);

        return true;
    }

    auto Application::Shutdown() const -> void {
        AssetManager::Get().Shutdown();
        SceneManager::Get().Shutdown();
        JobSystem::Get().Shutdown();
        ScriptManager::Get().ShutDown();
        Renderer::Shutdown();
        RHI::Shutdown();
        Window::Shutdown();
    }

    auto Application::MainLoop() -> void {
        OPTICK_START_CAPTURE();

        while (!_close_requested) {
            OPTICK_FRAME("MainThread");
            FRAME_PROFILER_BEGIN();

            Window::PollEvents();
            Time::Update();
            Log::FlushEvents();
            OnUpdate();
            OnDraw();

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

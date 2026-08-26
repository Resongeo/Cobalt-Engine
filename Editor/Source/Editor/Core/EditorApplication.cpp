// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/AssetEditors/AssetEditorManager.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/ECS/Systems/ScriptStartSystem.hpp"
#include "Engine/ECS/Systems/ScriptUpdateSystem.hpp"
#include "Engine/Events/EventBus.hpp"
#include "Engine/Events/KeyboardEvents.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Cobalt
{
    auto EditorApplication::OnBegin() -> void {
        AssetEditorManager::Get().Init();
        Gui::Init();
        Gui::SetupStyle();

        auto& scene_manager = SceneManager::Get();
        scene_manager.AddSystem<EditorRenderSystem>(Schedule::EditorUpdate);
        scene_manager.AddSystem<EditorRenderSystem>(Schedule::RuntimeUpdate);
        scene_manager.AddSystem<ScriptStartSystem>(Schedule::RuntimeStart);
        scene_manager.AddSystem<ScriptUpdateSystem>(Schedule::RuntimeUpdate);

        Window::Get().SetNativeEventCallback([](void* event) {
            const auto* sdl_event = static_cast<SDL_Event*>(event);
            Gui::ProcessEvent(sdl_event);
        });

        AssetEditorManager::Get().OpenEditor(SceneManager::Get().GetActiveSceneUUID());
    }

    auto EditorApplication::OnUpdate() -> void {
        auto& scene_manager = SceneManager::Get();
        _state.active_scene = scene_manager.GetActiveScene();

        scene_manager.Update();

        AssetEditorManager::Get().Update(_state);
    }

    void EditorApplication::OnDraw() {
        Gui::BeginFrame();
        {
            DrawMainDockSpace();
            DrawMainMenuBar();

            AssetEditorManager::Get().Draw(_state);

            if (_show_metrics_window) {
                ImGui::ShowMetricsWindow(&_show_metrics_window);
            }
        }
        Gui::EndFrame();
    }

    auto EditorApplication::DrawMainDockSpace() -> void {
        _state.main_dockspace_window_class.ClassId = ImGui::GetID("MainDockSpace_DockSpace");
        _state.main_dockspace_window_class.DockingAllowUnclassed = false;

        const auto dockspace_id = ImGui::GetID("MainDockSpace");
        const auto viewport = ImGui::GetMainViewport();
        constexpr auto flags = ImGuiDockNodeFlags_PassthruCentralNode;
        _state.main_dockspace_id = ImGui::DockSpaceOverViewport(dockspace_id, viewport, flags, &_state.main_dockspace_window_class);
    }

    auto EditorApplication::DrawMainMenuBar() -> void {
        auto& scene_manager = SceneManager::Get();

        ImGui::BeginMainMenuBar();
        {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                    AssetManager::Get().SaveAsset(SceneManager::Get().GetActiveSceneUUID());
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene")) {
                if (ImGui::MenuItem("Start Scene", "F5", nullptr, scene_manager.GetState() == SceneState::None)) {
                    _state.active_scene = _state.active_scene->Clone();
                    scene_manager.SetActiveScene(_state.active_scene);
                    scene_manager.SetState(SceneState::Start);
                }

                if (ImGui::MenuItem("Stop Scene", "F6", nullptr, scene_manager.GetState() == SceneState::Update)) {
                    scene_manager.SetActiveScene(scene_manager.GetActiveSceneUUID());
                    scene_manager.SetState(SceneState::None);
                    _state.active_scene = scene_manager.GetActiveScene();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {
                if (ImGui::MenuItem("ImGui Metrics")) {
                    _show_metrics_window = !_show_metrics_window;
                }

                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }
} // namespace Cobalt

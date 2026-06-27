// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Panels/LogPanel.hpp"
#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/ECS/Systems/ScriptStartSystem.hpp"
#include "Engine/ECS/Systems/ScriptUpdateSystem.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

#include "Editor/Gui/Widgets.hpp"

namespace Cobalt
{
    auto EditorApplication::OnBegin(EngineContext& ctx) -> void {
        Gui::Init(ctx.window);
        Gui::SetupStyle();

        _renderer.Init(10000, ctx.project.EditorAssetsPath());

        _state.framebuffer.Create(Vector{FramebufferAttachmentType::RGBA8}, Vec2(1600, 900), 1);
        _state.framebuffer.Unbind();

        ctx.scene_manager.AddSystem<EditorRenderSystem>(Schedule::EditorUpdate, &_renderer, &_state.editor_camera, &_state.framebuffer);
        ctx.scene_manager.AddSystem<EditorRenderSystem>(Schedule::RuntimeUpdate, &_renderer, &_state.editor_camera, &_state.framebuffer);

        ctx.scene_manager.AddSystem<ScriptStartSystem>(Schedule::RuntimeStart);
        ctx.scene_manager.AddSystem<ScriptUpdateSystem>(Schedule::RuntimeUpdate);

        _panels.emplace_back(Memory::MakeBox<AssetBrowserPanel>());
        _panels.emplace_back(Memory::MakeBox<EntityComponentsPanel>());
        _panels.emplace_back(Memory::MakeBox<SceneHierarchyPanel>());
        _panels.emplace_back(Memory::MakeBox<ViewportPanel>());
        _panels.emplace_back(Memory::MakeBox<LogPanel>());

        ctx.window.SetNativeEventCallback([](void* event) {
            const auto* sdl_event = static_cast<SDL_Event*>(event);
            Gui::ProcessEvent(sdl_event);
        });

        for (const auto& panel : _panels) {
            panel->Begin(ctx, _state);
        }
    }

    auto EditorApplication::OnUpdate(EngineContext& ctx) -> void {
        auto& scene_manager = ctx.scene_manager;
        scene_manager.Update(ctx);

        Gui::BeginFrame(ctx);
        {
            ImGui::BeginMainMenuBar();
            {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                        ctx.asset_manager.SaveAsset(ctx, ctx.scene_manager.GetActiveSceneUUID());
                    }

                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();

            if (const auto active_scene = scene_manager.GetActiveScene(); active_scene != nullptr) {
                const auto viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);

                constexpr auto dock_space_flags = 0 | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking |
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoBringToFrontOnFocus;

                ImGui::Begin("ViewsDockSpace", nullptr, dock_space_flags);
                {
                    const auto dock_space_id = ImGui::GetID("ViewsDockSpace");
                    ImGui::DockSpace(dock_space_id);
                }
                ImGui::End();

                _state.active_scene = active_scene;
                for (const auto& panel : _panels) {
                    panel->Draw(ctx, _state);
                }
            }
        }

        ImGui::Begin("Debug");
        {
            switch (ctx.scene_manager.GetState()) {
                case SceneState::None: {
                    if (ImGui::Button("Play")) {
                        _state.active_scene = _state.active_scene->Clone();
                        ctx.scene_manager.SetActiveScene(_state.active_scene);
                        ctx.scene_manager.SetState(SceneState::Start);
                    }
                    break;
                }
                case SceneState::Update: {
                    if (ImGui::Button("Stop")) {
                        ctx.scene_manager.SetActiveScene(ctx, ctx.scene_manager.GetActiveSceneUUID());
                        ctx.scene_manager.SetState(SceneState::None);
                        _state.active_scene = ctx.scene_manager.GetActiveScene();
                    }
                    break;
                }
                case SceneState::Start: break;
            }

            if (Widgets::Button("Log")) {
                CORE_INFO("ASD");
                CORE_WARN("ASD");
                CORE_ERROR("ASD");
                CORE_CRITICAL("ASD");
            }
        }
        ImGui::End();

        ImGui::Begin("Assets");
        {
            for (auto [id, meta] : ctx.asset_manager.GetRegistry()) {
                auto name_string = meta.path.string();
                ImGui::Text("Name %s", name_string.c_str());
                ImGui::PushID(id.value);
                ImGui::InputScalar("UUID", ImGuiDataType_U64, (void*)&id.value);
                ImGui::PopID();
            }
        }
        ImGui::End();

        Gui::EndFrame(ctx);
    }
} // namespace Cobalt

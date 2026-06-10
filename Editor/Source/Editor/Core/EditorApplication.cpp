// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/ECS/Systems/ScriptStartSystem.hpp"
#include "Engine/ECS/Systems/ScriptUpdateSystem.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Cobalt
{
    auto EditorApplication::begin(EngineContext& ctx) -> void {
        Gui::init(ctx.window);
        Gui::setup_style();

        m_renderer.init(10000, ctx.project.get_editor_assets_path());

        m_state.framebuffer.create(Vector{FramebufferAttachmentType::RGBA8}, Vec2(1600, 900), 1);
        m_state.framebuffer.unbind();

        ctx.scene_manager.add_system<EditorRenderSystem>(Schedule::EditorUpdate, &m_renderer, &m_state.editor_camera, &m_state.framebuffer);
        ctx.scene_manager.add_system<EditorRenderSystem>(Schedule::RuntimeUpdate, &m_renderer, &m_state.editor_camera, &m_state.framebuffer);

        ctx.scene_manager.add_system<ScriptStartSystem>(Schedule::RuntimeStart);
        ctx.scene_manager.add_system<ScriptUpdateSystem>(Schedule::RuntimeUpdate);

        m_panels.emplace_back(Memory::make_box<AssetBrowserPanel>());
        m_panels.emplace_back(Memory::make_box<EntityComponentsPanel>());
        m_panels.emplace_back(Memory::make_box<SceneHierarchyPanel>());
        m_panels.emplace_back(Memory::make_box<ViewportPanel>());

        ctx.window.set_native_event_callback([](void* event) {
            const auto* sdl_event = static_cast<SDL_Event*>(event);
            Gui::process_event(sdl_event);
        });

        for (const auto& panel : m_panels) {
            panel->begin(ctx, m_state);
        }
    }

    auto EditorApplication::update(EngineContext& ctx) -> void {
        auto& scene_manager = ctx.scene_manager;
        scene_manager.update(ctx);

        Gui::begin_frame(ctx);
        {
            ImGui::BeginMainMenuBar();
            {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                        ctx.asset_manager.save_asset(ctx, ctx.scene_manager.get_active_scene_uuid());
                    }

                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();

            if (const auto active_scene = scene_manager.get_active_scene(); active_scene != nullptr) {
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

                m_state.active_scene = active_scene;
                for (const auto& panel : m_panels) {
                    panel->draw(ctx, m_state);
                }
            }
        }

        ImGui::Begin("Debug");
        {
            switch (ctx.scene_manager.get_state()) {
                case SceneState::None: {
                    if (ImGui::Button("Play")) {
                        m_state.active_scene = m_state.active_scene->clone();
                        ctx.scene_manager.set_active_scene(m_state.active_scene);
                        ctx.scene_manager.set_state(SceneState::Start);
                    }
                    break;
                }
                case SceneState::Update: {
                    if (ImGui::Button("Stop")) {
                        ctx.scene_manager.set_active_scene(ctx, ctx.scene_manager.get_active_scene_uuid());
                        ctx.scene_manager.set_state(SceneState::None);
                        m_state.active_scene = ctx.scene_manager.get_active_scene();
                    }
                    break;
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Assets");

        for (auto [id, meta] : ctx.asset_manager.get_registry()) {
            auto name_string = meta.path.string();
            ImGui::Text("Name %s", name_string.c_str());
            ImGui::PushID(id.value);
            ImGui::InputScalar("UUID", ImGuiDataType_U64, (void*)&id.value);
            ImGui::PopID();
        }

        ImGui::End();

        Gui::end_frame(ctx);
    }
} // namespace Cobalt

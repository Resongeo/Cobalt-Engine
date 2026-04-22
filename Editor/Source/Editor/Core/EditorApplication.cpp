// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Core/Types/Color.hpp"
#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Cobalt
{
    auto EditorApplication::begin(EngineContext& ctx) -> void {
        Gui::init(ctx.window);
        Gui::setup_style();

        ctx.scene_manager.create_default_scene();

        m_renderer.init(10000, ctx.project.get_editor_assets_path());

        m_state.framebuffer.create(Vector{FramebufferAttachmentType::RGBA8}, Vec2(1600, 900), 1);
        m_state.framebuffer.unbind();

        ctx.scene_manager.add_system<EditorRenderSystem>(Schedule::EditorUpdate, &m_renderer, &m_state.editor_camera,
                                                         &m_state.framebuffer);

        m_panels.emplace_back(Memory::make_box<AssetBrowserPanel>());
        m_panels.emplace_back(Memory::make_box<EntityComponentsPanel>());
        m_panels.emplace_back(Memory::make_box<SceneHierarchyPanel>());
        m_panels.emplace_back(Memory::make_box<ViewportPanel>());

        for (const auto& panel : m_panels) {
            panel->begin(ctx, m_state);
        }
    }

    void EditorApplication::on_sdl_event(SDL_Event* event) {
        Gui::process_event(event);
    }

    auto EditorApplication::update(EngineContext& ctx) -> void {
        auto& scene_manager = ctx.scene_manager;
        scene_manager.update(ctx);

        Gui::begin_frame(ctx);
        {
            if (const auto active_scene = scene_manager.get_active_scene(); active_scene != nullptr) {
                const auto viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);

                constexpr auto dock_space_flags = 0 | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking |
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;

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

        Widgets::begin("Debug", {8, 8});
        {
            if (Widgets::button("Button")) {
                Logger::warn("Editor", "Button pressed!");
            }

            if (Widgets::button("Convert Color")) {
                auto [r, g, b, _] = Color::from_oklch(0.7f, 0.1, 77);
                Logger::warn("a", "rgb({}, {}, {})", r, g, b);
            }

            ImGui::Separator();

            if (Widgets::button("Default", Variant::Default)) {
                Logger::warn("Button", "I am a default button");
            }
            if (Widgets::button("Primary", Variant::Primary)) {
                Logger::warn("Button", "I am a primary button");
            }
            if (Widgets::button("Secondary", Variant::Secondary)) {
                Logger::warn("Button", "I am a secondary button");
            }
        }
        Widgets::end();

        //ImGui::ShowStyleEditor();

        Gui::end_frame(ctx);
    }
} // namespace Cobalt

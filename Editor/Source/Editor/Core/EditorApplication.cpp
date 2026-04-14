// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Panels/AssetBrowserPanel.hpp"
#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/Platform/Window.hpp"
#include "Engine/Scene/SceneManager.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

namespace Cobalt
{
    EditorApplication::EditorApplication(const CommandLineArgs args) : m_args(args) {}

    auto EditorApplication::on_begin() -> void {
        // TODO: Move project to Engine
        Project::init();
        Project::parse(m_args);

        const auto asset_dir = Project::get_project_assets_path();
        if (!std::filesystem::exists(asset_dir)) {
            std::filesystem::create_directories(asset_dir);
        }

        for (auto& entry : std::filesystem::recursive_directory_iterator(asset_dir)) {
            if (entry.is_directory()) {
                continue;
            }

            const auto entry_string = entry.path().string();
            const auto is_registered = AssetManager::instance().is_asset_registered(entry.path());
            Logger::warn("Editor", "Checking if {} is registered ... {}", entry_string, is_registered ? "YES" : "NO");
        }

        Gui::init(Window::instance());

        SceneManager::instance().create_default_scene();

        m_renderer.init(10000, Project::get_editor_assets_path());

        m_state.framebuffer.create(Vector{FramebufferAttachmentType::RGBA8}, Vec2(1600, 900), 1);
        m_state.framebuffer.unbind();

        SceneManager::instance().add_system<EditorRenderSystem>(Schedule::EditorUpdate, &m_renderer,
                                                                &m_state.editor_camera, &m_state.framebuffer);

        m_panels.emplace_back(Memory::make_box<AssetBrowserPanel>());
        m_panels.emplace_back(Memory::make_box<EntityComponentsPanel>());
        m_panels.emplace_back(Memory::make_box<SceneHierarchyPanel>());
        m_panels.emplace_back(Memory::make_box<ViewportPanel>());

        for (const auto& panel : m_panels) {
            panel->begin(m_state);
        }
    }

    void EditorApplication::on_sdl_event(SDL_Event* event) {
        Gui::process_event(event);
    }

    auto EditorApplication::on_update() -> void {
        auto& scene_manager = SceneManager::instance();
        scene_manager.update();

        Gui::begin_frame();
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
                panel->draw(m_state);
            }
        }

        Gui::end_frame();
    }
} // namespace Cobalt

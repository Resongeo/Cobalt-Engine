// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Core/Project.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Panels/EntityComponentsPanel.hpp"
#include "Editor/Gui/Panels/SceneHierarchyPanel.hpp"
#include "Editor/Gui/Panels/ViewportPanel.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/ECS/Components/TransformComponent.hpp"
#include "Engine/ECS/Systems/EditorRenderSystem.hpp"
#include "Engine/ECS/Systems/Schedule.hpp"
#include "Engine/Scene/Scene.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    EditorApplication::EditorApplication(const i32 argc, char* argv[])
        : m_argc(argc), m_argv(argv) {
    }

    auto EditorApplication::on_begin() -> void {
        Project::init();
        Project::parse(m_argc, m_argv);
        Gui::init(Application::get_window());

        Application::get_scene_manager().create_default_scene();

        m_renderer.init(3, Project::get_editor_assets_path());

        m_state.framebuffer.create(
            Vector{ Engine::FramebufferAttachmentType::RGBA8 },
            Vec2(1600, 900),
            1
        );
        m_state.framebuffer.unbind();
        
        Application::get_scene_manager().add_system<Engine::EditorRenderSystem>(
            Engine::Schedule::EditorUpdate,
            &m_renderer, &m_state.editor_camera, &m_state.framebuffer
        );

        m_panels.emplace_back(Memory::make_box<EntityComponentsPanel>());
        m_panels.emplace_back(Memory::make_box<SceneHierarchyPanel>());
        m_panels.emplace_back(Memory::make_box<ViewportPanel>());
    }

    auto EditorApplication::on_update() -> void {
        auto& scene_manager = Application::get_scene_manager();
        scene_manager.update();

        Gui::begin_frame();
        if (const auto active_scene = scene_manager.active_scene(); active_scene != nullptr) {
            const auto viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            
            constexpr auto dock_space_flags = 0
                | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoBringToFrontOnFocus;

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

    auto EditorApplication::on_end() -> void {
    }
}
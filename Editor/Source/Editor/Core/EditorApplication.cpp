// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Scene/Scene.hpp"

#include <imgui.h>

namespace Cobalt::Editor
{
    EditorApplication::EditorApplication(const i32 argc, char* argv[])
        : m_project(argc, argv) {
    }

    auto EditorApplication::on_begin() -> void {
        m_project.parse();

        Gui::init(p_window);
    }

    auto EditorApplication::on_update() -> void {
        Gui::begin_frame();
        {
            ImGui::Begin("Project");
            ImGui::Text("Name: %s", m_project.name().c_str());
            ImGui::Text("Version: %s", m_project.version().c_str());
            ImGui::End();

            ImGui::Begin("Scenes");
            if (const auto* scene = p_scene_manager.active_scene(); scene == nullptr) {
                ImGui::Text("Default scene");
            } else {
                const auto scenes = p_scene_manager.scenes();
                for (usize i = 0; i < scenes.size(); i++) {
                    ImGui::Text("%d %s", i, scenes[i].name.c_str());
                }
            }
            ImGui::End();
        }
        Gui::end_frame();
    }

    auto EditorApplication::on_end() -> void {
    }
}

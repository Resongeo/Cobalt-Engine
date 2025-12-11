// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Gui/Gui.hpp"
#include "Editor/Core/Project.hpp"

#include <glad/gl.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include <GLFW/glfw3.h>

namespace Cobalt::Editor
{
    auto Gui::init(const Engine::Window& window) -> void {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
        ImGui_ImplOpenGL3_Init("#version 450 core");

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    auto Gui::begin_frame() -> void {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        // TODO: Temporary fix. IO.Filename gets overwritten somewhere
        auto& io = ImGui::GetIO();
        const auto layout_ini = Project::get_editor_assets_path() / "Editor" / "Settings" / "DefaultLayout.ini";
        io.IniFilename = layout_ini.c_str();


        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    auto Gui::end_frame() -> void {
        glClearColor(0.08, 0.08, 0.08, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (const auto& io = ImGui::GetIO(); io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}

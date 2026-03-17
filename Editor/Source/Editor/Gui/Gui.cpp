// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Gui.hpp"
#include "Editor/Core/Project.hpp"
#include "Engine/Platform/Window.hpp"

#include <glad/gl.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include <SDL3/SDL.h>

namespace Cobalt::Editor
{
    auto Gui::init(const Engine::Window& window) -> void {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        const auto main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        ImGui_ImplSDL3_InitForOpenGL(window.get_handle(), window.get_gl_context());
        ImGui_ImplOpenGL3_Init("#version 450 core");

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }

    auto Gui::begin_frame() -> void {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();

        // TODO: Temporary fix. IO.Filename gets overwritten somewhere
        auto& io = ImGui::GetIO();
        const auto layout_ini = Project::get_editor_assets_path() / "Editor" / "Settings" / "DefaultLayout.ini";
        io.IniFilename = layout_ini.c_str();

        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    auto Gui::process_event(const SDL_Event* event) -> void {
        ImGui_ImplSDL3_ProcessEvent(event);
    }

    auto Gui::end_frame() -> void {
        const auto& io = ImGui::GetIO();

        ImGui::Render();
        glViewport(0, 0, static_cast<i32>(io.DisplaySize.x), static_cast<i32>(io.DisplaySize.y));
        glClearColor(0.08, 0.08, 0.08, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto* backup_current_window = Engine::Window::instance().get_handle();
            const auto backup_current_context = Engine::Window::instance().get_gl_context();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }
}

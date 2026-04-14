// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Gui.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Platform/Window.hpp"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glad/gl.h>
#include <imgui.h>

// IMPORTANT: Include ImGuizmo after imgui.h
#include <ImGuizmo.h>

namespace Cobalt
{
    auto Gui::init(const Window& window) -> void {
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

    auto Gui::begin_frame(EngineContext& ctx) -> void {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();

        // TODO: Temporary fix. IO.Filename gets overwritten somewhere
        auto& io = ImGui::GetIO();
        const auto layout_ini = ctx.project.get_project_assets_path().parent_path() / "Settings" / "DefaultLayout.ini";

        if (!std::filesystem::exists(layout_ini.parent_path())) {
            std::filesystem::create_directories(layout_ini.parent_path());
        }

        const auto layout_init_str = layout_ini.string();
        io.IniFilename = layout_init_str.c_str();

        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    auto Gui::process_event(const SDL_Event* event) -> void {
        ImGui_ImplSDL3_ProcessEvent(event);
    }

    auto Gui::end_frame(EngineContext& ctx) -> void {
        const auto& io = ImGui::GetIO();

        ImGui::Render();
        glViewport(0, 0, static_cast<i32>(io.DisplaySize.x), static_cast<i32>(io.DisplaySize.y));
        glClearColor(0.08, 0.08, 0.08, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto* backup_current_window = ctx.window.get_handle();
            const auto backup_current_context = ctx.window.get_gl_context();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/Colors.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Types/Color.hpp"
#include "Engine/Platform/Window.hpp"

#include "Editor/Embedded/Fonts/InterRegular.embed"
#include "Editor/Embedded/Fonts/InterSemibold.embed"
#include "Editor/Embedded/Fonts/InterBold.embed"
#include "Editor/Embedded/Icons/Lucide.embed"

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

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        auto& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);
        style.FontScaleDpi = main_scale;

        static constexpr ImWchar icon_ranges[] = { ICON_RANGE_MIN, ICON_RANGE_MAX, 0 };
        auto icon_font_config = ImFontConfig{};
        icon_font_config.MergeMode = false;
        icon_font_config.GlyphMinAdvanceX = 20.0f;
        icon_font_config.GlyphOffset = {0, 0};
        icon_font_config.GlyphRanges = icon_ranges;
        icon_font_config.PixelSnapV = true;

        Fonts::regular = io.Fonts->AddFontFromMemoryCompressedBase85TTF(inter_regular_base85);
        Fonts::semibold = io.Fonts->AddFontFromMemoryCompressedBase85TTF(inter_semibold_base85);
        Fonts::bold = io.Fonts->AddFontFromMemoryCompressedBase85TTF(inter_bold_base85);
        io.FontDefault = Fonts::regular;

        Fonts::icon = io.Fonts->AddFontFromMemoryCompressedBase85TTF(
            lucide_fonticon_base85,
            20.0f,
            &icon_font_config
        );

        ImGui_ImplSDL3_InitForOpenGL(window.get_handle(), window.get_gl_context());
        ImGui_ImplOpenGL3_Init("#version 450 core");
    }

    auto Gui::setup_style() -> void {
        auto& style = ImGui::GetStyle();

        // Default ImGui style values
        // -- Main --
        style.WindowPadding = {0.0f, 0.0f};
        style.FramePadding = {12.0f, 8.0f};
        style.ItemSpacing = {4.0f, 4.0f};
        style.GrabMinSize = 16.0f;

        // -- Borders --
        style.FrameBorderSize = 1.0f;
        style.WindowBorderSize = 0.0f;

        // -- Rounding --
        style.WindowRounding = 12.0f;
        style.FrameRounding = 6.0f;

        // -- Windows --
        style.WindowTitleAlign = {0.5f, 0.5f};

        // Default ImGui colors
        // -- Background colors --
        style.Colors[ImGuiCol_WindowBg]            = IMVEC4(Color::from_oklch(0.2f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_TitleBg]             = IMVEC4(Color::from_oklch(0.2f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_TitleBgActive]       = IMVEC4(Color::from_oklch(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_Button]              = IMVEC4(Color::from_oklch(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_ButtonHovered]       = IMVEC4(Color::from_oklch(0.28f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_ButtonActive]        = IMVEC4(Color::from_oklch(0.32f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_FrameBg]             = IMVEC4(Color::from_oklch(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_FrameBgActive]       = IMVEC4(Color::from_oklch(0.28f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_FrameBgHovered]      = IMVEC4(Color::from_oklch(0.28f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_Tab]                 = IMVEC4(Color::from_oklch(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_TabDimmedSelected]   = IMVEC4(Color::from_oklch(0.25f, 0.0f, 0.0f));

        // -- Foreground colors --
        style.Colors[ImGuiCol_Text]                = IMVEC4(Color::from_scalar(1.0f, 0.88f));

        // -- Primary colors --
        style.Colors[ImGuiCol_TabSelected]         = IMVEC4(Color::from_oklch(0.52f, 0.17f, 260.0f));
        style.Colors[ImGuiCol_TabSelectedOverline] = IMVEC4(Color::from_oklch(0.52f, 0.17f, 260.0f));
        style.Colors[ImGuiCol_TabHovered]          = IMVEC4(Color::from_oklch(0.61f, 0.17f, 260.0f));
    }

    auto Gui::begin_frame(const EngineContext& ctx) -> void {
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

    auto Gui::end_frame(const EngineContext& ctx) -> void {
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

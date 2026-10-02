// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Gui.hpp"
#include "Editor/Gui/Colors.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Editor/Gui/Fonts.hpp"
#include "Editor/Gui/Textures.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Types/Color.hpp"
#include "Engine/Rendering/RHI.hpp"

#include "Editor/Embedded/Fonts/InterBold.embed"
#include "Editor/Embedded/Fonts/InterRegular.embed"
#include "Editor/Embedded/Fonts/InterSemibold.embed"
#include "Editor/Embedded/Icons/Lucide.embed"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>
#include <imgui.h>

// IMPORTANT: Include ImGuizmo after imgui.h
#include <ImGuizmo.h>

namespace Cobalt
{
    auto Gui::Init() -> void {
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

        // NOTE: Copied from examples
        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForSDLGPU(Window::GetHandle());
        ImGui_ImplSDLGPU3_InitInfo init_info = {};
        init_info.Device = RHI::GetDevice();
        init_info.ColorTargetFormat = RHI::GetSwapchainTextureFormat();
        init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1; // Only used in multi-viewports mode.
        init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR; // Only used in multi-viewports mode.
        init_info.PresentMode = RHI::GetPresentMode();
        ImGui_ImplSDLGPU3_Init(&init_info);

        auto LoadFontWithIcon = [](const ImGuiIO& io, const char* font_data, const ImFontConfig* font_config) -> ImFont* {
            const auto font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(font_data);
            io.Fonts->AddFontFromMemoryCompressedBase85TTF(lucide_base85, 24, font_config);
            return font;
        };

        auto icon_font_config = ImFontConfig{};
        icon_font_config.MergeMode = true;
        icon_font_config.GlyphMinAdvanceX = 20.0f;
        icon_font_config.GlyphOffset = {0, 4};
        icon_font_config.PixelSnapV = true;

        Fonts::regular = LoadFontWithIcon(io, inter_regular_base85, &icon_font_config);
        Fonts::semibold = LoadFontWithIcon(io, inter_semibold_base85, &icon_font_config);
        Fonts::bold = LoadFontWithIcon(io, inter_bold_base85, &icon_font_config);
        io.FontDefault = Fonts::regular;

        const auto editor_asset_path = Project::GetEditorAssetsPath();
        Textures::directory.LoadFromFile(editor_asset_path / "Textures" / "Directory.png");
        Textures::placeholder.LoadFromFile(editor_asset_path / "Textures" / "Default.png");
        Textures::script.LoadFromFile(editor_asset_path / "Textures" / "Script.png");
        Textures::sprite.LoadFromFile(editor_asset_path / "Textures" / "Texture.png");
    }

    auto Gui::Shutdown() -> void {
        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplSDLGPU3_Shutdown();
        ImGui::DestroyContext();

        Textures::directory.Destroy();
        Textures::placeholder.Destroy();
        Textures::script.Destroy();
        Textures::sprite.Destroy();
    }

    auto Gui::SetupStyle() -> void {
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
        style.Colors[ImGuiCol_WindowBg] = IMVEC4(Color::FromOKLCH(0.2f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_TitleBg] = IMVEC4(Color::FromOKLCH(0.2f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_TitleBgActive] = IMVEC4(Color::FromOKLCH(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_Button] = IMVEC4(Color::FromOKLCH(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_ButtonHovered] = IMVEC4(Color::FromOKLCH(0.28f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_ButtonActive] = IMVEC4(Color::FromOKLCH(0.32f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_FrameBg] = IMVEC4(Color::FromOKLCH(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_FrameBgActive] = IMVEC4(Color::FromOKLCH(0.28f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_FrameBgHovered] = IMVEC4(Color::FromOKLCH(0.28f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_Tab] = IMVEC4(Color::FromOKLCH(0.25f, 0.0f, 0.0f));
        style.Colors[ImGuiCol_TabDimmedSelected] = IMVEC4(Color::FromOKLCH(0.25f, 0.0f, 0.0f));

        // -- Foreground colors --
        style.Colors[ImGuiCol_Text] = IMVEC4(Color::FromScalar(1.0f, 0.88f));

        // -- Primary colors --
        style.Colors[ImGuiCol_TabSelected] = IMVEC4(Color::FromOKLCH(0.52f, 0.17f, 260.0f));
        style.Colors[ImGuiCol_TabSelectedOverline] = IMVEC4(Color::FromOKLCH(0.52f, 0.17f, 260.0f));
        style.Colors[ImGuiCol_TabHovered] = IMVEC4(Color::FromOKLCH(0.61f, 0.17f, 260.0f));
    }

    auto Gui::BeginFrame() -> void {
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();

        // TODO: Temporary fix. IO.Filename gets overwritten somewhere
        auto& io = ImGui::GetIO();
        const auto layout_ini = Project::GetProjectAssetsPath().parent_path() / "Settings" / "DefaultLayout.ini";

        if (!std::filesystem::exists(layout_ini.parent_path())) {
            std::filesystem::create_directories(layout_ini.parent_path());
        }

        const auto layout_init_str = layout_ini.string();
        io.IniFilename = layout_init_str.c_str();

        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    auto Gui::ProcessEvent(const SDL_Event* event) -> void {
        ImGui_ImplSDL3_ProcessEvent(event);
    }

    auto Gui::EndFrame() -> void {
        const auto& io = ImGui::GetIO();

        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

        SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(RHI::GetDevice()); // Acquire a GPU command buffer

        SDL_GPUTexture* swapchain_texture;
        SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, Window::GetHandle(), &swapchain_texture, nullptr,
                                              nullptr); // Acquire a swapchain texture

        if (swapchain_texture != nullptr && !is_minimized) {
            // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
            ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

            // Setup and start a render pass
            SDL_GPUColorTargetInfo target_info = {};
            target_info.texture = swapchain_texture;
            target_info.clear_color = SDL_FColor{0.18, 0.18, 0.18, 1.0};
            target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            target_info.store_op = SDL_GPU_STOREOP_STORE;
            target_info.mip_level = 0;
            target_info.layer_or_depth_plane = 0;
            target_info.cycle = false;
            SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

            // Render ImGui
            ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

            SDL_EndGPURenderPass(render_pass);
        }

        // Submit the command buffer
        SDL_SubmitGPUCommandBuffer(command_buffer);
    }
} // namespace Cobalt

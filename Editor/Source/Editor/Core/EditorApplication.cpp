// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Editor/Gui/Gui.hpp"
#include "Engine/Core/Logger.hpp"

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
        ImGui::ShowDemoWindow();
        Gui::end_frame();
    }

    auto EditorApplication::on_end() -> void {
    }
}

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"

#include "Engine/Core/Logger.hpp"

namespace Cobalt::Editor
{
    EditorApplication::EditorApplication(const i32 argc, char* argv[])
        : m_project(argc, argv) {
    }

    auto EditorApplication::on_begin() -> void {
        m_project.parse();
    }

    auto EditorApplication::on_update() -> void {
    }

    auto EditorApplication::on_end() -> void {
    }
}

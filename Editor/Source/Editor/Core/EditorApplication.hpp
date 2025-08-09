// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Application.hpp"
#include "Editor/Core/Project.hpp"

namespace Cobalt::Editor
{
    class EditorApplication final : public Engine::Application
    {
    public:
        EditorApplication(i32 argc, char* argv[]);

        auto on_begin() -> void override;
        auto on_update() -> void override;
        auto on_end() -> void override;

    private:
        Project m_project;
    };
}
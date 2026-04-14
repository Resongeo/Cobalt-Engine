// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/CommandLineArgs.hpp"
#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt
{
    class Project final
    {
    public:
        auto init(const CommandLineArgs& cli_args) -> void;
        auto get_name() -> String&;
        auto get_version() -> String&;
        auto get_editor_assets_path() const -> Filepath;
        auto get_project_assets_path() const -> Filepath;

    private:
        Vector<String> m_args = {};
        Filepath m_project_path = {};
        Filepath m_editor_path = {};
        String m_name = {};
        String m_version = {};
    };
} // namespace Cobalt

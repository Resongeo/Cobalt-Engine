// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types.hpp"

namespace Cobalt::Editor
{
    class Project final
    {
    public:
        Project(i32 argc, char* argv[]);

        auto parse() -> void;

    private:
        Vector<String> m_args;
        Filepath m_project_path;

        String m_name = {};
        String m_version = {};
    };
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types.hpp"

namespace Cobalt::Editor
{
    class Project final
    {
    public:
        static auto init() -> void;
        static auto parse(i32 argc, char* argv[]) -> void;
        static auto get_name() -> String&;
        static auto get_version() -> String&;

    private:
        static Project* s_instance;
        Vector<String> m_args = {};
        Filepath m_project_path = {};
        String m_name = {};
        String m_version = {};
    };
}
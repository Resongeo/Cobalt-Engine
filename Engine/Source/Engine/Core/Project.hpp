// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/CommandLineArgs.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"

namespace Cobalt
{
    class Project final
    {
    public:
        auto Init(const CommandLineArgs& cli_args) -> void;
        auto Name() -> String&;
        auto Version() -> String&;
        auto EditorAssetsPath() const -> Filepath;
        auto ProjectAssetsPath() const -> Filepath;
        auto StartupSceneUUID() const -> UUID;

    private:
        Vector<String> _args = {};
        Filepath _project_path = {};
        Filepath _editor_path = {};
        String _name = {};
        String _version = {};
        UUID _startup_scene;
    };
} // namespace Cobalt

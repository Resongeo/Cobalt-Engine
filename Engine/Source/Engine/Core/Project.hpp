// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/CommandLineArgs.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"
#include "Engine/Core/Error.hpp"

namespace Cobalt
{
    enum class ProjectInitError
    {
        NoArgument,
        ProjectFileDoesntExists,
        ProjectFileCantParse,
        ProjectFileNotValid,
    };

    namespace Project
    {
        auto Initialize(const CommandLineArgs& cli_args) -> Result<void, ProjectInitError>;
        auto GetName() -> String&;
        auto GetVersion() -> String&;
        auto GetEditorAssetsPath() -> Filepath&;
        auto GetProjectAssetsPath() -> Filepath&;
        auto GetStartupSceneUUID() -> UUID;
    };
} // namespace Cobalt

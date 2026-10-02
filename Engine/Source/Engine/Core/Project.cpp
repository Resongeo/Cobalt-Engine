// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Project.hpp"
#include "Engine/Core/Log.hpp"

#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

namespace Cobalt
{
    struct ProjectState
    {
        Vector<String> args = {};
        Filepath project_path = {};
        Filepath project_asset_path = {};
        Filepath editor_path = {};
        Filepath editor_asset_path = {};
        String name = {};
        String version = {};
        UUID startup_scene = {};
    };
    static ProjectState state;

    auto Project::Initialize(const CommandLineArgs& cli_args) -> Result<void, ProjectInitError> {
        CORE_ASSERT(cli_args.count != 0, "No cli arguments were passed");
        CORE_ASSERT(cli_args.args != nullptr, "Cli arguments are nullptr");

        state.args = Vector<String>(cli_args.args, cli_args.args + cli_args.count);
        state.name = "No Project";
        state.version = "0.0.0";
        state.editor_path = Filepath(state.args[0].c_str()).parent_path();
        state.editor_asset_path = state.editor_path / "Assets";

        if (state.args.size() < 2) {
            return Err(ProjectInitError::NoArgument);
        }

        if (!std::filesystem::exists(state.args[1].c_str())) {
            return Err(ProjectInitError::ProjectFileDoesntExists);
        }

        auto valid_file = true;
        const auto project_file_path = Filepath(state.args[1].c_str());
        state.project_path = project_file_path.parent_path();
        state.project_asset_path = state.project_path / "Assets";

        if (project_file_path.extension() != ".cbproj") {
            valid_file = false;
        }

        const auto result = toml::parse_file(project_file_path.string());

        if (!result) {
            auto error_msg = std::ostringstream();
            error_msg << result.error();
            CORE_ERROR("Project: Could not parse {}: {}", project_file_path.string(), error_msg.str());
            return Err(ProjectInitError::ProjectFileCantParse);
        }

        if (!valid_file) {
            CORE_ERROR("Project: {} is not a valid project file", project_file_path.string());
            return Err(ProjectInitError::ProjectFileNotValid);
        }

        auto table = result.table();
        state.name = table["project"]["name"].value_or<const char*>("Default");
        state.version = table["project"]["version"].value_or<const char*>("0.0.0");
        state.startup_scene = UUID(table["project"]["startup_scene"].value_or<u64>(0));

        CORE_INFO("Core::Project: Project found! Name: {}. Version: {}", state.name, state.version);

        return {};
    }

    auto Project::GetName() -> String& {
        return state.name;
    }

    auto Project::GetVersion() -> String& {
        return state.version;
    }

    auto Project::GetEditorAssetsPath() -> Filepath& {
        return state.editor_asset_path;
    }

    auto Project::GetProjectAssetsPath() -> Filepath& {
        return state.project_asset_path;
    }

    auto Project::GetStartupSceneUUID() -> UUID {
        return state.startup_scene;
    }
} // namespace Cobalt

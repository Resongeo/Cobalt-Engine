// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Project.hpp"
#include "Engine/Core/Log.hpp"

#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

namespace Cobalt
{
    auto Project::Init(const CommandLineArgs& cli_args) -> void {
        _args = Vector<String>(cli_args.args, cli_args.args + cli_args.count);

        _name = "No Project";
        _version = "0.0.0";

        _editor_path = Filepath(_args[0]).parent_path();

        if (_args.size() < 2) {
            CORE_WARN("Project: No file is provided. Please provide a valid path to a .cbproj file");
            return;
        }

        if (!std::filesystem::exists(_args[1])) {
            CORE_ERROR("Project: File path does not exists: {}", _args[1]);
            return;
        }

        auto valid_file = true;
        const auto project_file_path = Filepath(_args[1]);
        _project_path = project_file_path.parent_path();

        if (project_file_path.extension() != ".cbproj") {
            valid_file = false;
        }

        const auto result = toml::parse_file(project_file_path.string());

        if (!result) {
            auto error_msg = std::ostringstream();
            error_msg << result.error();
            CORE_ERROR("Project: Could not parse {}: {}", project_file_path.string(), error_msg.str());
            valid_file = false;
        }

        if (!valid_file) {
            CORE_ERROR("Project: {} is not a valid project file", project_file_path.string());

            return;
        }

        auto table = result.table();
        _name = table["project"]["name"].value_or<String>("Default");
        _version = table["project"]["version"].value_or<String>("0.0.0");
        _startup_scene = UUID(table["project"]["startup_scene"].value_or<u64>(0));

        CORE_INFO("Project: Loading. Name: {}. Version: {}", _name, _version);
    }

    auto Project::GetName() -> String& {
        return _name;
    }

    auto Project::GetVersion() -> String& {
        return _version;
    }

    auto Project::GetEditorAssetsPath() const -> Filepath {
        return _editor_path / "Assets";
    }

    auto Project::GetProjectAssetsPath() const -> Filepath {
        return _project_path / "Assets";
    }

    auto Project::GetStartupSceneUUID() const -> UUID {
        return _startup_scene;
    }

    auto Project::Get() -> Project& {
        static Project instance;
        return instance;
    }
} // namespace Cobalt

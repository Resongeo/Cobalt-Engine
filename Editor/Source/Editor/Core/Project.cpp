// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/Project.hpp"
#include "Engine/Core/Logger.hpp"

#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

namespace Cobalt::Editor
{
    static Project* s_instance = nullptr;

    auto Project::init() -> void {
        s_instance = new Project();
    }

    auto Project::parse(const i32 argc, char* argv[]) -> void {
        s_instance->m_args = Vector<String>(argv, argv + argc);
        auto& args = s_instance->m_args;
        auto& name = s_instance->m_name;
        auto& version = s_instance->m_version;
        auto& project_path = s_instance->m_project_path;
        auto& editor_path = s_instance->m_editor_path;

        name = "No Project";
        version = "0.0.0";

        editor_path = Filepath(args[0]).parent_path();

        if (args.size() < 2) {
            Engine::Logger::warn(
                "Editor::Project",
                "No project file is provided. Please provide a valid path to a .cbproj file"
            );
            return;
        }

        if (!std::filesystem::exists(args[1])) {
            Engine::Logger::error(
                "Editor::Project",
                "Project file path does not exists: {}",
                args[1]
            );
            return;
        }

        // TODO: Load a Default level if no project is provided

        auto valid_file = true;
        const auto project_file_path = Filepath(args[1]);
        project_path = project_file_path.parent_path();

        if (project_file_path.extension() != ".cbproj") {
            valid_file = false;
        }

        const auto result = toml::parse_file(project_file_path.string());

        if (!result) {
            auto error_msg = std::ostringstream();
            error_msg << result.error();
            Engine::Logger::error(
                "Editor::Project",
                "Error while parsing {}: {}",
                project_file_path.string(), error_msg.str()
            );
            valid_file = false;
        }

        if (!valid_file) {
            Engine::Logger::error(
                "Editor::Project",
                "{} is not a valid project file",
                project_file_path.string()
            );

            return;
        }

        auto table = result.table();
        name = table["project"]["name"].value_or<String>("Default");
        version = table["project"]["version"].value_or<String>("0.0.0");

        Engine::Logger::trace("Editor::Project", "Loading project.\n  Name: {}\n  Version: {}",
            name, version
        );
    }

    auto Project::get_name() -> String& {
        return s_instance->m_name;
    }

    auto Project::get_version() -> String& {
        return s_instance->m_version;
    }

    auto Project::get_editor_assets_path() -> Filepath {
        return s_instance->m_editor_path / "Assets";
    }

    auto Project::get_project_assets_path() -> Filepath {
        return s_instance->m_project_path / "Assets";
    }
}

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Project.hpp"
#include "Engine/Core/Logger.hpp"

#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

namespace Cobalt
{
    auto Project::init(const CommandLineArgs& cli_args) -> void {
        m_args = Vector<String>(cli_args.args, cli_args.args + cli_args.count);

        m_name = "No Project";
        m_version = "0.0.0";

        m_editor_path = Filepath(m_args[0]).parent_path();

        if (m_args.size() < 2) {
            Logger::warn("Editor::Project",
                         "No project file is provided. Please provide a valid path to a .cbproj file");
            return;
        }

        if (!std::filesystem::exists(m_args[1])) {
            Logger::error("Editor::Project", "Project file path does not exists: {}", m_args[1]);
            return;
        }

        auto valid_file = true;
        const auto project_file_path = Filepath(m_args[1]);
        m_project_path = project_file_path.parent_path();

        if (project_file_path.extension() != ".cbproj") {
            valid_file = false;
        }

        const auto result = toml::parse_file(project_file_path.string());

        if (!result) {
            auto error_msg = std::ostringstream();
            error_msg << result.error();
            Logger::error("Editor::Project", "Error while parsing {}: {}", project_file_path.string(), error_msg.str());
            valid_file = false;
        }

        if (!valid_file) {
            Logger::error("Editor::Project", "{} is not a valid project file", project_file_path.string());

            return;
        }

        auto table = result.table();
        m_name = table["project"]["name"].value_or<String>("Default");
        m_version = table["project"]["version"].value_or<String>("0.0.0");
        m_startup_scene = UUID(table["project"]["startup_scene"].value_or<u64>(0));

        Logger::trace("Editor::Project", "Loading project.\n  Name: {}\n  Version: {}", m_name, m_version);
    }

    auto Project::get_name() -> String& {
        return m_name;
    }

    auto Project::get_version() -> String& {
        return m_version;
    }

    auto Project::get_editor_assets_path() const -> Filepath {
        return m_editor_path / "Assets";
    }

    auto Project::get_project_assets_path() const -> Filepath {
        return m_project_path / "Assets";
    }

    auto Project::get_startup_scene_uuid() const -> UUID {
        return m_startup_scene;
    }
} // namespace Cobalt

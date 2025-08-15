// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/Project.hpp"
#include "Engine/Core/Logger.hpp"

#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>

namespace Cobalt::Editor
{
    Project::Project(const i32 argc, char* argv[]) {
        m_args = Vector<String>(argv, argv + argc);
    }

    auto Project::parse() -> void {
        m_name = "No Project";
        m_version = "0.0.0";

        if (m_args.size() < 2) {
            Engine::Logger::warn(
                "Editor::Project",
                "No project file is provided. Please provide a valid path to a .cbproj file"
            );
            return;
        }

        if (!std::filesystem::exists(m_args[1])) {
            Engine::Logger::error(
                "Editor::Project",
                "Project file path does not exists: {}",
                m_args[1]
            );
            return;
        }

        // TODO: Load a Default level if no project is provided

        auto valid_file = true;
        m_project_path = Filepath(m_args[1]);

        if (m_project_path.extension() != ".cbproj") {
            valid_file = false;
        }

        const auto result = toml::parse_file(m_project_path.string());

        if (!result) {
            auto error_msg = std::ostringstream();
            error_msg << result.error();
            Engine::Logger::error(
                "Editor::Project",
                "Error while parsing {}: {}",
                m_project_path.string(), error_msg.str()
            );
            valid_file = false;
        }

        if (!valid_file) {
            Engine::Logger::error(
                "Editor::Project",
                "{} is not a valid project file",
                m_project_path.string()
            );

            return;
        }

        auto table = result.table();
        m_name = table["project"]["name"].value_or<String>("Default");
        m_version = table["project"]["version"].value_or<String>("0.0.0");
        m_startup_scene = UUID(table["project"]["startup-scene"].value_or<u64>(0));

        Engine::Logger::trace("Editor::Project", "Loading project.\n  Name: {}\n  Version: {}",
            m_name, m_version
        );
    }

    auto Project::name() -> String {
        return m_name;
    }

    auto Project::version() -> String {
        return m_version;
    }

    auto Project::startup_scene() const -> UUID {
        return m_startup_scene;
    }
}

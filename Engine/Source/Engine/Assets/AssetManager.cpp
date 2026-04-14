// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/AssetManager.hpp"

#include <ranges>

namespace Cobalt
{
    auto AssetManager::init(const Project& project) -> void {
        const auto& project_asset_dir = project.get_project_assets_path();

        if (!std::filesystem::exists(project_asset_dir)) {
            std::filesystem::create_directories(project_asset_dir);
        }

        // TODO: Load and parse asset registry file

        for (auto& entry : std::filesystem::recursive_directory_iterator(project_asset_dir)) {
            if (entry.is_directory()) {
                continue;
            }

            const auto entry_string = entry.path().string();
            if (!is_asset_registered(entry.path())) {
                // TODO: Register asset
            }
        }
    }

    auto AssetManager::is_asset_registered(const UUID id) -> bool {
        const auto it = m_asset_registry.find(id);
        return it != m_asset_registry.end();
    }

    auto AssetManager::is_asset_registered(const Filepath& path) -> bool {
        for (const auto& asset : m_asset_registry | std::views::values) {
            if (asset.path == path) {
                return true;
            }
        }

        return false;
    }
} // namespace Cobalt

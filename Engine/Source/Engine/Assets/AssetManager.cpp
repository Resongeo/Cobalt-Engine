// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/AssetManager.hpp"

#include <ranges>

namespace Cobalt
{
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

    auto AssetManager::instance() -> AssetManager& {
        static AssetManager instance;
        return instance;
    }
} // namespace Cobalt

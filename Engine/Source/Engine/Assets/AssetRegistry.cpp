// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/AssetRegistry.hpp"

namespace Cobalt
{
    auto AssetRegistry::GetMetadata(const Filepath& filepath) const -> Optional<AssetMetadata> {
        if (const auto uuid = GetUUID(filepath); uuid.has_value()) {
            return GetMetadata(uuid.value());
        }

        return None;
    }

    auto AssetRegistry::GetMetadata(const UUID uuid) const -> Optional<AssetMetadata> {
        if (const auto it = _registry.find(uuid); it != _registry.end()) {
            return it->second;
        }

        return None;
    }

    auto AssetRegistry::GetUUID(const Filepath& filepath) const -> Optional<UUID> {
        if (const auto it = _path_lookup.find(filepath); it != _path_lookup.end()) {
            return it->second;
        }

        return None;
    }

    auto AssetRegistry::RegisterAsset(const AssetMetadata& metadata) -> void {
        const auto uuid = metadata.uuid;
        _registry[uuid] = metadata;

        if (!metadata.path.empty()) {
            _path_lookup[metadata.path] = uuid;
        }
    }
} // namespace Cobalt

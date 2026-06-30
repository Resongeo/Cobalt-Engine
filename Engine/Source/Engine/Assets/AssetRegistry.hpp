// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"
#include "Engine/Assets/AssetMetadata.hpp"

namespace Cobalt
{
    class AssetRegistry final
    {
    public:
        auto GetMetadata(const Filepath& filepath) const -> Optional<AssetMetadata>;
        auto GetMetadata(UUID uuid) const -> Optional<AssetMetadata>;
        auto GetUUID(const Filepath& filepath) const -> Optional<UUID>;

        auto RegisterAsset(const AssetMetadata& metadata) -> void;

    private:
        HashMap<UUID, AssetMetadata> _registry = {};
        HashMap<Filepath, UUID> _path_lookup = {};
    };
}
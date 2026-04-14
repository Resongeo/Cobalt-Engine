// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/AssetMetadata.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Types/UUID.hpp"

namespace Cobalt
{
    class AssetManager final
    {
    public:
        auto init(const Project& project) -> void;

        auto is_asset_registered(UUID id) -> bool;
        auto is_asset_registered(const Filepath& path) -> bool;

        template <typename T>
        auto get(const UUID id) const -> Rc<T> {
            if (const auto it = m_loaded_assets.find(id); it != m_loaded_assets.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            // TODO: Load asset
            return nullptr;
        }

    private:
        HashMap<UUID, Rc<void>> m_loaded_assets = {};
        HashMap<UUID, AssetMetadata> m_asset_registry = {};
    };
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAssetLoader.hpp"
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

        auto register_asset(const Filepath& path) const -> void;
        auto register_asset(UUID id, const AssetMetadata& metadata) const -> void;
        auto get_metadata(UUID id) const -> AssetMetadata;
        auto is_asset_registered(UUID id) const -> bool;
        auto is_asset_registered(const Filepath& path) const -> bool;
        auto load_registry() const -> void;
        auto save_registry() const -> void;
        auto get_registry() const -> HashMap<UUID, AssetMetadata>&;

        template <typename T>
        auto get_asset(const UUID id) const -> Rc<T> {
            if (!id.is_valid()) {
                return nullptr;
            }

            if (const auto it = m_loaded.find(id); it != m_loaded.end()) {
                return std::static_pointer_cast<T>(it->second);
            }

            const auto& metadata = get_metadata(id);
            const auto& loader = m_loaders[static_cast<usize>(metadata.type)];

            if (!loader) {
                return nullptr;
            }

            if (const auto asset = loader->load(metadata)) {
                m_loaded[id] = asset;
                return std::static_pointer_cast<T>(asset);
            }

            return nullptr;
        }

    private:
        auto get_asset_type_from_extension(const Filepath& path) const -> AssetType;
        auto asset_type_to_string(AssetType type) const -> String;
        auto string_to_asset_type(const String& str) const -> AssetType;
        auto is_file_asset(const Filepath& path) const -> bool;

    private:
        mutable HashMap<UUID, Rc<void>> m_loaded = {};
        mutable HashMap<UUID, AssetMetadata> m_registry = {};
        Array<Rc<IAssetLoader>, static_cast<usize>(AssetType::SIZE)> m_loaders = {};
        Filepath m_registry_path = {};
        Filepath m_assets_dir = {};
    };
} // namespace Cobalt

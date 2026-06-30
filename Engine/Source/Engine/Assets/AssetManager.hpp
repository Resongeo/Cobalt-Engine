// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAssetSerializer.hpp"
#include "Engine/Assets/AssetRegistry.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Types/UUID.hpp"
#include "Engine/Platform/DialogManager.hpp"

namespace Cobalt
{
    class AssetManager final
    {
    public:
        auto Init() -> void;

        auto RegisterAsset(const Filepath& path) -> void;
        auto IsAssetRegistered(UUID uuid) const -> bool;
        auto IsAssetRegistered(const Filepath& path) const -> bool;
        auto GetRegistry() const -> const AssetRegistry&;

        auto LoadRegistry() -> void;
        auto SaveRegistry() const -> void;

        static auto GetAssetTypeFromExtension(const Filepath& path) -> AssetType;

        auto SaveAsset(UUID uuid) -> bool;

        template <typename T>
        auto GetAsset(const UUID uuid) const -> Rc<T> {
            if (!uuid.IsValid()) {
                return nullptr;
            }

            if (const auto it = _loaded.find(uuid); it != _loaded.end()) {
                return std::static_pointer_cast<T>(it->second);
            }

            const auto metadata_opt = _registry.GetMetadata(uuid);
            if (!metadata_opt.has_value()) {
                return nullptr;
            }

            const auto& metadata = metadata_opt.value();
            const auto& serializer = _serializers[static_cast<usize>(metadata.type)];

            if (!serializer) {
                return nullptr;
            }

            if (const auto asset = serializer->Deserialize(metadata)) {
                _loaded[uuid] = asset;
                return std::static_pointer_cast<T>(asset);
            }

            return nullptr;
        }

        template <typename T>
        auto CreateInMemoryAsset(const AssetType type) -> UUID {
            auto asset = Memory::MakeRc<T>();
            const auto uuid = UUID::Generate();
            const auto meta = AssetMetadata{
                .uuid = uuid,
                .type = type,
                .is_memory = true
            };

            _registry.RegisterAsset(meta);
            _loaded[uuid] = asset;

            return uuid;
        }

        static auto Get() -> AssetManager&;

    private:
        auto AssetTypeToString(AssetType type) const -> String;
        auto AssetTypeToFilters(AssetType type) const -> Vector<DialogFileFilter>;
        auto StringToAssetType(const String& str) const -> AssetType;
        auto IsFileAsset(const Filepath& path) const -> bool;

    private:
        mutable HashMap<UUID, Rc<IAsset>> _loaded = {};
        AssetRegistry _registry = {};
        Array<Rc<IAssetSerializer>, static_cast<usize>(AssetType::SIZE)> _serializers = {};
        Filepath _registry_path = {};
        Filepath _assets_dir = {};
    };
} // namespace Cobalt

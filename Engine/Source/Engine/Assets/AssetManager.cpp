// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Assets/Serializers/SceneSerializer.hpp"
#include "Engine/Assets/Serializers/ScriptSerializer.hpp"
#include "Engine/Assets/Serializers/Texture2DSerializer.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/File.hpp"

#include <fstream>
#include <ranges>
#include <simdjson.h>

#define TOML_EXCEPTIONS 0
#include <toml++/toml.hpp>
#include <SDL3/SDL.h>

namespace Cobalt
{
    auto AssetManager::Init() -> void {
        _assets_dir = Project::Get().GetProjectAssetsPath();

        if (!File::Exists(_assets_dir)) {
            std::filesystem::create_directories(_assets_dir);
        }

        _registry_path = _assets_dir / "AssetRegistry.json";

        LoadRegistry();

        _serializers[static_cast<usize>(AssetType::Texture)] = Memory::MakeRc<Texture2DSerializer>();
        _serializers[static_cast<usize>(AssetType::Script)] = Memory::MakeRc<ScriptSerializer>();
        _serializers[static_cast<usize>(AssetType::Scene)] = Memory::MakeRc<SceneSerializer>();
    }

    auto AssetManager::RegisterAsset(const Filepath& path) -> void {
        auto meta = AssetMetadata{};

        if (IsFileHasMetadata(path)) {
            meta = LoadMetadata(path);
        } else {
            meta = AssetMetadata{
                .uuid = UUID::Generate(),
                .path = path,
                .type = GetAssetTypeFromExtension(path)
            };
            SaveMetadata(meta);
        }

        _registry.RegisterAsset(meta);
    }

    auto AssetManager::IsAssetRegistered(const UUID id) const -> bool {
        return _registry.GetMetadata(id).has_value();
    }

    auto AssetManager::IsAssetRegistered(const Filepath& path) const -> bool {
        return _registry.GetUUID(path).has_value();
    }

    auto AssetManager::LoadRegistry() -> void {
        for (auto& entry : std::filesystem::recursive_directory_iterator(_assets_dir)) {
            if (entry.is_directory()) {
                continue;
            }

            if (IsFileAsset(entry.path())) {
                RegisterAsset(entry.path());
            }
        }
    }

    auto AssetManager::GetRegistry() const -> const AssetRegistry& {
        return _registry;
    }

    auto AssetManager::GetAssetTypeFromExtension(const Filepath& path) -> AssetType {
        const auto extension = path.extension().string();

        if (extension == ".as") {
            return AssetType::Script;
        }
        if (extension == ".cbscene") {
            return AssetType::Scene;
        }
        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
            return AssetType::Texture;
        }

        return AssetType::None;
    }

    auto AssetManager::SaveAsset(const UUID uuid) const -> bool {
        auto metadata_opt = _registry.GetMetadata(uuid);
        if (!metadata_opt.has_value()) {
            return false;
        }

        auto& meta = metadata_opt.value();
        const auto serializer = _serializers[static_cast<usize>(meta.type)];

        if (!serializer) {
            return false;
        }

        if (meta.is_memory || meta.path.empty()) {
            struct DialogSync {
                Filepath path = {};
                bool completed = false;
            };
            auto sync_data = Memory::MakeRc<DialogSync>();

            static auto filter = AssetTypeToFilters(meta.type);
            const auto default_path = Project::Get().GetProjectAssetsPath().string();

            DialogManager::Get().ShowSaveDialog(default_path, filter, [sync_data](const Filepath& chosen_path) {
                sync_data->path = chosen_path;
                sync_data->completed = true;
            });

            // TODO: Handle async better. I made this blocking for now
            while (!sync_data->completed) {
                SDL_PumpEvents();
                SDL_Delay(16);
            }

            meta.path = sync_data->path;
            meta.is_memory = false;
        }

        if (meta.path.empty()) {
            return false;
        }

        return serializer->Serialize(_loaded[uuid], meta);
    }

    auto AssetManager::Get() -> AssetManager& {
        static AssetManager instance;
        return instance;
    }

    auto AssetManager::AssetTypeToString(const AssetType type) const -> String {
        switch (type) {
            case AssetType::Texture: return "Texture";
            case AssetType::Scene: return "Scene";
            case AssetType::Script: return "Script";
            case AssetType::None:
            default: return "None";
        }
    }

    auto AssetManager::AssetTypeToFilters(const AssetType type) const -> Vector<DialogFileFilter> {
        switch (type) {
            case AssetType::Texture: return {{.name = "Texture", .pattern = "png;jpg"}};
            case AssetType::Scene: return {{.name = "Cobalt Scene", .pattern = "cbscene"}};
            case AssetType::Script: return {{.name = "Angel Script", .pattern = "as"}};
            case AssetType::None:
            default: return {};
        }
    }

    auto AssetManager::StringToAssetType(const String& str) const -> AssetType {
        if (str == "Texture") {
            return AssetType::Texture;
        }
        if (str == "Scene") {
            return AssetType::Scene;
        }
        if (str == "Script") {
            return AssetType::Script;
        }

        return AssetType::None;
    }

    auto AssetManager::IsFileAsset(const Filepath& path) const -> bool {
        return GetAssetTypeFromExtension(path) != AssetType::None;
    }

    auto AssetManager::IsFileHasMetadata(const Filepath& path) const -> bool {
        if (GetAssetTypeFromExtension(path) == AssetType::None) {
            return false;
        }

        auto meta_path = path;
        meta_path += ".meta";

        return File::Exists(meta_path);
    }

    auto AssetManager::LoadMetadata(const Filepath& path) const -> AssetMetadata {
        auto meta_path = path;
        meta_path += ".meta";
        auto meta_path_str = meta_path.string();

        auto result = toml::parse_file(meta_path_str);

        if (!result) {
            auto error_msg = std::ostringstream();
            error_msg << result.error();
            CORE_ERROR("AssetManager: Could not parse {}: {}", meta_path_str, error_msg.str());
        }

        auto meta = AssetMetadata{};
        auto table = result.table();
        auto meta_table = table["asset metadata"];

        meta.uuid = UUID(std::stoull(meta_table["uuid"].value_or<String>("0")));
        meta.type = StringToAssetType(meta_table["type"].value_or<String>("None"));
        meta.path = path;

        return meta;
    }

    auto AssetManager::SaveMetadata(const AssetMetadata& meta) const -> void {
        auto meta_path = meta.path;
        meta_path += ".meta";

        const auto table = toml::table{
            {"asset metadata", toml::table{
                {"uuid", std::to_string(meta.uuid.value)},
                {"type", AssetTypeToString(meta.type)},
                /* TODO: Save last_modified when implementing asset reloading */
            }}
        };

        auto serialized_meta_table = std::ostringstream();
        serialized_meta_table << table;
        const auto result = serialized_meta_table.str();

        File::Save(meta_path, result);
    }
} // namespace Cobalt

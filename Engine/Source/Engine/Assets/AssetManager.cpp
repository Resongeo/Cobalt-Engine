// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Assets/Serializers/SceneSerializer.hpp"
#include "Engine/Assets/Serializers/ScriptSerializer.hpp"
#include "Engine/Assets/Serializers/Texture2DSerializer.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Project.hpp"

#include <fstream>
#include <ranges>
#include <simdjson.h>

#include <SDL3/SDL.h>

namespace Cobalt
{
    auto AssetManager::Init() -> void {
        _assets_dir = Project::Get().GetProjectAssetsPath();

        if (!std::filesystem::exists(_assets_dir)) {
            std::filesystem::create_directories(_assets_dir);
        }

        _registry_path = _assets_dir / "AssetRegistry.json";
        LoadRegistry();

        for (auto& entry : std::filesystem::recursive_directory_iterator(_assets_dir)) {
            if (entry.is_directory()) {
                continue;
            }

            if (IsFileAsset(entry.path())) {
                RegisterAsset(entry.path());
            }
        }

        _serializers[static_cast<usize>(AssetType::Texture)] = Memory::MakeRc<Texture2DSerializer>();
        _serializers[static_cast<usize>(AssetType::Script)] = Memory::MakeRc<ScriptSerializer>();
        _serializers[static_cast<usize>(AssetType::Scene)] = Memory::MakeRc<SceneSerializer>();
    }

    auto AssetManager::RegisterAsset(const Filepath& path) const -> void {
        if (IsAssetRegistered(path)) {
            return;
        }

        if (!IsFileAsset(path)) {
            return;
        }

        if (!std::filesystem::exists(path)) {
            return;
        }

        const auto meta = AssetMetadata{.path = path, .type = GetAssetTypeFromExtension(path)};

        _registry[UUID::Generate()] = meta;
    }

    auto AssetManager::RegisterAsset(const UUID id, const AssetMetadata& metadata) const -> void {
        if (!std::filesystem::exists(metadata.path)) {
            return;
        }

        _registry[id] = metadata;
    }

    auto AssetManager::GetMetadata(const UUID id) const -> AssetMetadata {
        if (const auto it = _registry.find(id); it != _registry.end()) {
            return it->second;
        }

        return {};
    }

    auto AssetManager::GetUUID(const Filepath& path) const -> UUID {
        for (auto& [id, meta] : _registry) {
            if (meta.path == path) {
                return id;
            }
        }

        return UUID{};
    }

    auto AssetManager::IsAssetRegistered(const UUID id) const -> bool {
        const auto it = _registry.find(id);
        return it != _registry.end();
    }

    auto AssetManager::IsAssetRegistered(const Filepath& path) const -> bool {
        for (const auto& asset : _registry | std::views::values) {
            if (asset.path == path) {
                return true;
            }
        }

        return false;
    }

    auto AssetManager::LoadRegistry() const -> void {
        if (!std::filesystem::exists(_registry_path)) {
            return;
        }

        simdjson::ondemand::parser parser;
        const auto json = simdjson::padded_string::load(_registry_path.string());
        auto doc = simdjson::ondemand::document{};

        if (const auto error = parser.iterate(json).get(doc)) {
            CORE_ERROR("AssetManager: {}", simdjson::error_message(error));
            return;
        }

        auto assets = simdjson::ondemand::array{};
        if (const auto error = doc["assets"].get_array().get(assets)) {
            CORE_ERROR("AssetManager: {} Expected: \"assets\"", simdjson::error_message(error));
            return;
        }

        for (auto asset : assets) {
            auto id = UUID{};
            auto meta = AssetMetadata{};

            if (const auto error = asset["uuid"].get_uint64().get(id.value)) {
                CORE_ERROR("AssetManager: {} Expected: \"{}\"", simdjson::error_message(error), "uuid");

                continue;
            }

            auto relative_path_string = String{};
            if (const auto error = asset["path"].get_string().get(relative_path_string)) {
                CORE_ERROR("AssetManager: {} Expected: \"{}\"", simdjson::error_message(error), "path");

                continue;
            }
            meta.path = _assets_dir / relative_path_string;

            auto type_string = String{};
            if (const auto error = asset["type"].get_string().get(type_string)) {
                CORE_ERROR("AssetManager: {} Expected: \"{}\"", simdjson::error_message(error), "type");

                continue;
            }
            meta.type = StringToAssetType(type_string);

            RegisterAsset(id, meta);
        }
    }

    auto AssetManager::SaveRegistry() const -> void {
        auto sb = simdjson::builder::string_builder{};

        sb.start_object();
        {
            sb.escape_and_append_with_quotes("assets");
            sb.append_colon();

            sb.start_array();
            {
                auto asset_index = 0;
                for (const auto& [id, meta] : _registry) {
                    if (meta.is_memory) {
                        asset_index++;
                        continue;
                    }

                    asset_index++;
                    sb.start_object();
                    {
                        sb.append_key_value("uuid", id.value);
                        sb.append_comma();

                        const auto relative_path = std::filesystem::relative(meta.path, _assets_dir).string();
                        sb.append_key_value("path", relative_path);
                        sb.append_comma();

                        sb.append_key_value("type", AssetTypeToString(meta.type));
                    }
                    sb.end_object();

                    if (asset_index < _registry.size()) {
                        sb.append_comma();
                    }
                }
            }
            sb.end_array();
        }
        sb.end_object();

        const auto result = sb.view();

        std::ofstream out(_registry_path);
        out << result.value_unsafe();
        out.close();
    }

    auto AssetManager::GetRegistry() const -> HashMap<UUID, AssetMetadata>& {
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

    auto AssetManager::SaveAsset(const UUID id) const -> bool {
        auto meta = GetMetadata(id);
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

        return serializer->Serialize(_loaded[id], meta);
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
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Assets/Serializers/SceneSerializer.hpp"
#include "Engine/Assets/Serializers/ScriptSerializer.hpp"
#include "Engine/Assets/Serializers/Texture2DSerializer.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Core/Log.hpp"

#include <fstream>
#include <ranges>
#include <simdjson.h>

#include <SDL3/SDL.h>

namespace Cobalt
{
    auto AssetManager::init(const Project& project) -> void {
        m_assets_dir = project.get_project_assets_path();

        if (!std::filesystem::exists(m_assets_dir)) {
            std::filesystem::create_directories(m_assets_dir);
        }

        m_registry_path = m_assets_dir / "AssetRegistry.json";
        load_registry();

        for (auto& entry : std::filesystem::recursive_directory_iterator(m_assets_dir)) {
            if (entry.is_directory()) {
                continue;
            }

            if (is_file_asset(entry.path())) {
                register_asset(entry.path());
            }
        }

        m_serializers[static_cast<usize>(AssetType::Texture)] = Memory::make_rc<Texture2DSerializer>();
        m_serializers[static_cast<usize>(AssetType::Script)] = Memory::make_rc<ScriptSerializer>();
        m_serializers[static_cast<usize>(AssetType::Scene)] = Memory::make_rc<SceneSerializer>();
    }

    auto AssetManager::register_asset(const Filepath& path) const -> void {
        if (is_asset_registered(path)) {
            return;
        }

        if (!is_file_asset(path)) {
            return;
        }

        if (!std::filesystem::exists(path)) {
            return;
        }

        const auto meta = AssetMetadata{.path = path, .type = get_asset_type_from_extension(path)};

        m_registry[UUID::generate()] = meta;
    }

    auto AssetManager::register_asset(const UUID id, const AssetMetadata& metadata) const -> void {
        if (!std::filesystem::exists(metadata.path)) {
            return;
        }

        m_registry[id] = metadata;
    }

    auto AssetManager::get_metadata(const UUID id) const -> AssetMetadata {
        if (const auto it = m_registry.find(id); it != m_registry.end()) {
            return it->second;
        }

        return {};
    }

    auto AssetManager::get_uuid(const Filepath& path) const -> UUID {
        for (auto& [id, meta] : m_registry) {
            if (meta.path == path) {
                return id;
            }
        }

        return UUID{};
    }

    auto AssetManager::is_asset_registered(const UUID id) const -> bool {
        const auto it = m_registry.find(id);
        return it != m_registry.end();
    }

    auto AssetManager::is_asset_registered(const Filepath& path) const -> bool {
        for (const auto& asset : m_registry | std::views::values) {
            if (asset.path == path) {
                return true;
            }
        }

        return false;
    }

    auto AssetManager::load_registry() const -> void {
        if (!std::filesystem::exists(m_registry_path)) {
            return;
        }

        simdjson::ondemand::parser parser;
        const auto json = simdjson::padded_string::load(m_registry_path.string());
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
            meta.path = m_assets_dir / relative_path_string;

            auto type_string = String{};
            if (const auto error = asset["type"].get_string().get(type_string)) {
                CORE_ERROR("AssetManager: {} Expected: \"{}\"", simdjson::error_message(error), "type");

                continue;
            }
            meta.type = string_to_asset_type(type_string);

            register_asset(id, meta);
        }
    }

    auto AssetManager::save_registry() const -> void {
        auto sb = simdjson::builder::string_builder{};

        sb.start_object();
        {
            sb.escape_and_append_with_quotes("assets");
            sb.append_colon();

            sb.start_array();
            {
                auto asset_index = 0;
                for (const auto& [id, meta] : m_registry) {
                    if (meta.is_memory) {
                        asset_index++;
                        continue;
                    }

                    asset_index++;
                    sb.start_object();
                    {
                        sb.append_key_value("uuid", id.value);
                        sb.append_comma();

                        const auto relative_path = std::filesystem::relative(meta.path, m_assets_dir).string();
                        sb.append_key_value("path", relative_path);
                        sb.append_comma();

                        sb.append_key_value("type", asset_type_to_string(meta.type));
                    }
                    sb.end_object();

                    if (asset_index < m_registry.size()) {
                        sb.append_comma();
                    }
                }
            }
            sb.end_array();
        }
        sb.end_object();

        const auto result = sb.view();

        std::ofstream out(m_registry_path);
        out << result.value_unsafe();
        out.close();
    }

    auto AssetManager::get_registry() const -> HashMap<UUID, AssetMetadata>& {
        return m_registry;
    }

    auto AssetManager::get_asset_type_from_extension(const Filepath& path) -> AssetType {
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

    auto AssetManager::save_asset(EngineContext& ctx, const UUID id) const -> bool {
        auto meta = get_metadata(id);
        const auto serializer = m_serializers[static_cast<usize>(meta.type)];

        if (!serializer) {
            return false;
        }

        if (meta.is_memory || meta.path.empty()) {
            struct DialogSync {
                Filepath path = {};
                bool completed = false;
            };
            auto sync_data = Memory::make_rc<DialogSync>();

            static auto filter = asset_type_to_filters(meta.type);
            const auto default_path = ctx.project.get_project_assets_path().string();

            ctx.dialog_manager.show_save_dialog(default_path, filter, [sync_data](const Filepath& chosen_path) {
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

        return serializer->serialize(m_loaded[id], meta);
    }

    auto AssetManager::asset_type_to_string(const AssetType type) const -> String {
        switch (type) {
            case AssetType::Texture: return "Texture";
            case AssetType::Scene: return "Scene";
            case AssetType::Script: return "Script";
            case AssetType::None:
            default: return "None";
        }
    }

    auto AssetManager::asset_type_to_filters(const AssetType type) const -> Vector<DialogFileFilter> {
        switch (type) {
            case AssetType::Texture: return {{.name = "Texture", .pattern = "png;jpg"}};
            case AssetType::Scene: return {{.name = "Cobalt Scene", .pattern = "cbscene"}};
            case AssetType::Script: return {{.name = "Angel Script", .pattern = "as"}};
            case AssetType::None:
            default: return {};
        }
    }

    auto AssetManager::string_to_asset_type(const String& str) const -> AssetType {
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

    auto AssetManager::is_file_asset(const Filepath& path) const -> bool {
        return get_asset_type_from_extension(path) != AssetType::None;
    }
} // namespace Cobalt

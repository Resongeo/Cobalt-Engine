// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/AssetManager.hpp"
#include "Engine/Core/Logger.hpp"

#include <ranges>
#include <simdjson.h>
#include <fstream>

namespace Cobalt
{
    auto AssetManager::init(const Project& project) -> void {
        m_assets_path = project.get_project_assets_path();

        if (!std::filesystem::exists(m_assets_path)) {
            std::filesystem::create_directories(m_assets_path);
        }

        m_registry_path = m_assets_path / "AssetRegistry.json";
        load_registry();

        for (auto& entry : std::filesystem::recursive_directory_iterator(m_assets_path)) {
            if (entry.is_directory()) {
                continue;
            }

            if (auto asset_path = std::filesystem::relative(entry.path(), m_assets_path); is_file_asset(asset_path)) {
                register_asset(asset_path);
            }
        }
    }

    auto AssetManager::register_asset(const Filepath& path) -> void {
        if (is_asset_registered(path)) {
            return;
        }

        const auto meta = AssetMetadata{
            .path = path,
            .type = get_asset_type_from_extension(path)
        };

        m_asset_registry[UUID::generate()] = meta;
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

    auto AssetManager::load_registry() -> void {
        if (!std::filesystem::exists(m_registry_path)) {
            return;
        }

        simdjson::ondemand::parser parser;
        const auto json = simdjson::padded_string::load(m_registry_path.string());
        auto doc = simdjson::ondemand::document{};

        if (const auto error = parser.iterate(json).get(doc)) {
            Logger::error("Engine::AssetManager", "{}",
                simdjson::error_message(error)
            );
            return;
        }

        auto assets = simdjson::ondemand::array{};
        if (const auto error = doc["assets"].get_array().get(assets)) {
            Logger::error("Engine::AssetManager", "{} Expected: \"assets\"",
                simdjson::error_message(error)
            );
            return;
        }

        for (auto asset : assets) {
            auto id = UUID{};
            auto meta = AssetMetadata{};

            if (const auto error = asset["uuid"].get_uint64().get(id.value)) {
                Logger::error(
                    "Engine::AssetManager", "{} Expected: \"{}\"",
                    simdjson::error_message(error), "uuid"
                );

                continue;
            }

            auto path_string = String{};
            if (const auto error = asset["path"].get_string().get(path_string)) {
                Logger::error(
                    "Engine::AssetManager", "{} Expected: \"{}\"",
                    simdjson::error_message(error), "path"
                );

                continue;
            }
            meta.path = path_string;

            auto type_string = String{};
            if (const auto error = asset["type"].get_string().get(type_string)) {
                Logger::error(
                    "Engine::AssetManager", "{} Expected: \"{}\"",
                    simdjson::error_message(error), "type"
                );

                continue;
            }
            meta.type = string_to_asset_type(type_string);

            m_asset_registry[id] = meta;
        }
    }

    auto AssetManager::save_registry() -> void {
        auto sb = simdjson::builder::string_builder{};

        sb.start_object();
        {
            sb.escape_and_append_with_quotes("assets");
            sb.append_colon();

            sb.start_array();
            {
                auto asset_index = 0;
                for (const auto& [id, meta] : m_asset_registry) {
                    asset_index++;
                    sb.start_object();
                    {
                        sb.append_key_value("uuid", id.value);
                        sb.append_comma();

                        sb.append_key_value("path", meta.path.string());
                        sb.append_comma();

                        sb.append_key_value("type", asset_type_to_string(meta.type));
                    }
                    sb.end_object();

                    if (asset_index != m_asset_registry.size()) {
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

    auto AssetManager::get_asset_type_from_extension(const Filepath& path) const -> AssetType {
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

    auto AssetManager::asset_type_to_string(const AssetType type) const -> String {
        switch (type) {
            case AssetType::Texture: return "Texture";
            case AssetType::Scene: return "Scene";
            case AssetType::Script: return "Script";
            case AssetType::None:
            default: return "None";
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

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/Serializers/SceneSerializer.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Types/UUID.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Scene/Scene.hpp"

#include <simdjson.h>

#include <charconv>
#include <fstream>

namespace Cobalt
{
    namespace Helpers
    {
        static auto color_to_json_object(simdjson::builder::string_builder& sb, const String& name, const Color& col) -> void {
            sb.escape_and_append_with_quotes(name);
            sb.append_colon();
            sb.start_object();
            {
                sb.append_key_value("r", std::to_string(col.r));
                sb.append_comma();
                sb.append_key_value("g", std::to_string(col.g));
                sb.append_comma();
                sb.append_key_value("b", std::to_string(col.b));
                sb.append_comma();
                sb.append_key_value("a", std::to_string(col.a));
            }
            sb.end_object();
        }

        static auto vec_to_json_object(simdjson::builder::string_builder& sb, const String& name, const Vec3& vec) -> void {
            sb.escape_and_append_with_quotes(name);
            sb.append_colon();
            sb.start_object();
            {
                sb.append_key_value("x", std::to_string(vec.x));
                sb.append_comma();
                sb.append_key_value("y", std::to_string(vec.y));
                sb.append_comma();
                sb.append_key_value("z", std::to_string(vec.z));
            }
            sb.end_object();
        }

        static auto vec_to_json_object(simdjson::builder::string_builder& sb, const String& name, const Vec2& vec) -> void {
            sb.escape_and_append_with_quotes(name);
            sb.append_colon();
            sb.start_object();
            {
                sb.append_key_value("x", std::to_string(vec.x));
                sb.append_comma();
                sb.append_key_value("y", std::to_string(vec.y));
            }
            sb.end_object();
        }

        static auto parse_float(simdjson::ondemand::object& obj, const StringView key, const f32 default_val = 0.0f) -> f32 {
            f32 value = default_val;
            StringView str_view;
            if (obj[key].get_string().get(str_view) == simdjson::SUCCESS) {
                std::from_chars(str_view.data(), str_view.data() + str_view.size(), value);
            }
            return value;
        }
    } // namespace Helpers

    auto SceneSerializer::deserialize(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> {
        auto scene = Memory::make_rc<Scene>();
        auto& registry = scene->get_registry();

        simdjson::ondemand::parser parser;
        const auto json = simdjson::padded_string::load(metadata.path.string());
        auto doc = simdjson::ondemand::document{};

        if (const auto error = parser.iterate(json).get(doc)) {
            CORE_ERROR("Assets::Loaders::SceneLoader: {}", simdjson::error_message(error));
            return nullptr;
        }

        StringView scene_name;
        if (const auto error = doc["name"].get_string().get(scene_name)) {
            CORE_ERROR("Assets::Loaders::SceneLoader: {} Expected: \"name\"", simdjson::error_message(error));
        } else {
            scene->set_name(std::string(scene_name));
        }

        simdjson::ondemand::array entities;
        if (const auto error = doc["entities"].get_array().get(entities)) {
            CORE_ERROR("Assets::Loaders::SceneLoader: {} Expected: \"entities\"", simdjson::error_message(error));
            return nullptr;
        }

        for (auto entity_res : entities) {
            simdjson::ondemand::object entity_obj;
            if (entity_res.get(entity_obj) != simdjson::SUCCESS) {
                continue;
            }

            const auto entity_id = registry.create();
            auto entity = Entity(entity_id, &registry);

            for (auto field : entity_obj) {
                StringView component_type = field.unescaped_key();
                auto value = field.value();

                if (component_type == "tag") {
                    simdjson::ondemand::object comp_obj;
                    if (value.get_object().get(comp_obj) == simdjson::SUCCESS) {
                        auto& tag = entity.add_component<TagComponent>();

                        u64 uuid_val = 0;
                        comp_obj["uuid"].get(uuid_val);
                        tag.uuid.value = uuid_val;

                        StringView name_view;
                        if (comp_obj["name"].get_string().get(name_view) == simdjson::SUCCESS) {
                            tag.name = std::string(name_view);
                        }
                    }
                } else if (component_type == "transform") {
                    simdjson::ondemand::object comp_obj;
                    if (value.get_object().get(comp_obj) == simdjson::SUCCESS) {
                        auto& transform = entity.add_component<TransformComponent>();

                        simdjson::ondemand::object pos_obj;
                        if (comp_obj["position"].get_object().get(pos_obj) == simdjson::SUCCESS) {
                            transform.position.x = Helpers::parse_float(pos_obj, "x", 0.0f);
                            transform.position.y = Helpers::parse_float(pos_obj, "y", 0.0f);
                        }

                        simdjson::ondemand::object scale_obj;
                        if (comp_obj["scale"].get_object().get(scale_obj) == simdjson::SUCCESS) {
                            transform.scale.x = Helpers::parse_float(scale_obj, "x", 1.0f);
                            transform.scale.y = Helpers::parse_float(scale_obj, "y", 1.0f);
                        }

                        StringView rot_view;
                        if (comp_obj["rotation"].get_string().get(rot_view) == simdjson::SUCCESS) {
                            float rot_val = 0.0f;
                            std::from_chars(rot_view.data(), rot_view.data() + rot_view.size(), rot_val);
                            transform.rotation = rot_val;
                        }
                    }
                } else if (component_type == "sprite") {
                    simdjson::ondemand::object comp_obj;
                    if (value.get_object().get(comp_obj) == simdjson::SUCCESS) {
                        auto& sprite = entity.add_component<SpriteComponent>();

                        simdjson::ondemand::object tint_obj;
                        if (comp_obj["tint"].get_object().get(tint_obj) == simdjson::SUCCESS) {
                            float r = Helpers::parse_float(tint_obj, "r", 1.0f);
                            float g = Helpers::parse_float(tint_obj, "g", 1.0f);
                            float b = Helpers::parse_float(tint_obj, "b", 1.0f);
                            float a = Helpers::parse_float(tint_obj, "a", 1.0f);
                            sprite.tint = Color{r, g, b, a};
                        }

                        u64 texture_uuid = 0;
                        comp_obj["uuid"].get(texture_uuid);
                        sprite.texture_id.value = texture_uuid;
                    }
                } else if (component_type == "script") {
                    simdjson::ondemand::object comp_obj;
                    if (value.get_object().get(comp_obj) == simdjson::SUCCESS) {
                        auto& script = entity.add_component<ScriptComponent>();

                        u64 script_uuid = 0;
                        comp_obj["uuid"].get(script_uuid);
                        script.script_id.value = script_uuid;
                    }
                }
            }
        }

        return scene;
    }

    auto SceneSerializer::serialize(const Rc<IAsset>& asset, const AssetMetadata& metadata) -> bool {
        const auto scene = std::static_pointer_cast<Scene>(asset);
        auto& registry = scene->get_registry();
        auto sb = simdjson::builder::string_builder{};

        sb.start_object();
        {
            sb.append_key_value("name", scene->get_name());
            sb.append_comma();

            sb.escape_and_append_with_quotes("entities");
            sb.append_colon();

            sb.start_array();
            {
                const auto view = registry.view<entt::entity>();
                bool first_entity = true;

                for (auto it = view.rbegin(); it != view.rend(); ++it) {
                    const auto entity_id = *it;
                    auto entity = Entity(entity_id, &registry);

                    if (!first_entity) {
                        sb.append_comma();
                    }
                    first_entity = false;

                    sb.start_object();
                    int active_components = 0;

                    if (entity.has_component<TagComponent>()) {
                        active_components++;

                        sb.escape_and_append_with_quotes("tag");
                        sb.append_colon();
                        sb.start_object();
                        {
                            const auto& tag = entity.get_component<TagComponent>();
                            sb.append_key_value("uuid", tag.uuid.value);
                            sb.append_comma();
                            sb.append_key_value("name", tag.name);
                        }
                        sb.end_object();
                    }

                    if (entity.has_component<TransformComponent>()) {
                        if (active_components > 0) {
                            sb.append_comma();
                        }
                        active_components++;

                        sb.escape_and_append_with_quotes("transform");
                        sb.append_colon();
                        sb.start_object();
                        {
                            const auto& transform = entity.get_component<TransformComponent>();
                            Helpers::vec_to_json_object(sb, "position", transform.position);
                            sb.append_comma();
                            Helpers::vec_to_json_object(sb, "scale", transform.scale);
                            sb.append_comma();
                            sb.append_key_value("rotation", std::to_string(transform.rotation));
                        }
                        sb.end_object();
                    }

                    if (entity.has_component<SpriteComponent>()) {
                        if (active_components > 0) {
                            sb.append_comma();
                        }
                        active_components++;

                        sb.escape_and_append_with_quotes("sprite");
                        sb.append_colon();
                        sb.start_object();
                        {
                            const auto& sprite = entity.get_component<SpriteComponent>();
                            Helpers::color_to_json_object(sb, "tint", sprite.tint);
                            sb.append_comma();
                            sb.append_key_value("uuid", sprite.texture_id.value);
                        }
                        sb.end_object();
                    }

                    if (entity.has_component<ScriptComponent>()) {
                        if (active_components > 0) {
                            sb.append_comma();
                        }
                        active_components++;

                        sb.escape_and_append_with_quotes("script");
                        sb.append_colon();
                        sb.start_object();
                        {
                            const auto& script = entity.get_component<ScriptComponent>();
                            sb.append_key_value("uuid", script.script_id.value);
                        }
                        sb.end_object();
                    }

                    sb.end_object();
                }
            }
            sb.end_array();
        }
        sb.end_object();

        const auto result = sb.view();

        std::ofstream out(metadata.path);
        out << result.value_unsafe();
        out.close();

        return true;
    }
} // namespace Cobalt

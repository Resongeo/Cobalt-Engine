// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scripting/ScriptGlue.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Math.hpp"
#include "Engine/ECS/Components/Minimal.hpp"
#include "Engine/Scripting/ScriptEntity.hpp"

namespace Cobalt
{
    namespace Utils
    {
        inline auto get_engine_ctx() -> EngineContext* {
            const auto* as_ctx = asGetActiveContext();
            if (!as_ctx) {
                return nullptr;
            }

            const auto* engine = as_ctx->GetEngine();
            return static_cast<EngineContext*>(engine->GetUserData());
        }

        static auto get_active_registry() -> entt::registry* {
            if (const auto* ctx = get_engine_ctx(); ctx) {
                return &ctx->scene_manager.get_active_scene()->get_registry();
            }

            return nullptr;
        }
    }

    auto test_print(String& msg) -> void {
        CORE_INFO("Script: {}", msg);
    }

    auto vec2_default_constructor(void* memory) -> void {
        new(memory)Vec2(0.0f);
    }

    auto vec2_constructor(const f32 x, const f32 y, void* memory) -> void {
        new(memory)Vec2(x, y);
    }

    auto vec2_scalar_constructor(const f32 scalar, void* memory) -> void {
        new(memory)Vec2(scalar);
    }

    auto entity_find_by_name(const String& name) -> ScriptEntity {
        auto* registry = Utils::get_active_registry();
        if (!registry) {
            return ScriptEntity{};
        }

        const auto view = registry->view<TagComponent>();
        for (auto entity : view) {
            if (view.get<TagComponent>(entity).name == name) {
                auto script_entity = ScriptEntity{};
                script_entity.entity = {entity, registry};

                return script_entity;
            }
        }

        return ScriptEntity{};
    }

    auto ScriptGlue::register_types(asIScriptEngine* engine) -> void {
        engine->RegisterObjectType("Entity", sizeof(ScriptEntity), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);

        engine->RegisterObjectType("vec2", sizeof(Vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_ALLFLOATS);
        engine->RegisterObjectProperty("vec2", "float x", asOFFSET(Vec2, x));
        engine->RegisterObjectProperty("vec2", "float y", asOFFSET(Vec2, y));
        engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(vec2_default_constructor), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(vec2_scalar_constructor), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(vec2_constructor), asCALL_CDECL_OBJLAST);
    }

    auto ScriptGlue::register_global_functions(asIScriptEngine* engine) -> void {
        engine->RegisterGlobalFunction("void Print(string& in)", asFUNCTION(test_print), asCALL_CDECL);

        engine->SetDefaultNamespace("Scene");
        engine->RegisterGlobalFunction("Entity find_by_name(string& in)", asFUNCTION(entity_find_by_name), asCALL_CDECL);
        engine->SetDefaultNamespace("");
    }

    auto ScriptGlue::register_entity_api(asIScriptEngine* engine) -> void {
        // -- Transform APIs --
        engine->RegisterObjectMethod("Entity", "void set_position(vec2& in)", asMETHOD(ScriptEntity, set_position), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_position_x(float)", asMETHOD(ScriptEntity, set_position_x), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_position_y(float)", asMETHOD(ScriptEntity, set_position_y), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "vec2 get_position()", asMETHOD(ScriptEntity, get_position), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_position_x()", asMETHOD(ScriptEntity, get_position_x), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_position_y()", asMETHOD(ScriptEntity, get_position_y), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float set_rotation(float)", asMETHOD(ScriptEntity, set_rotation), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_rotation()", asMETHOD(ScriptEntity, get_rotation), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_scale(vec2& in)", asMETHOD(ScriptEntity, set_scale), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_scale_x(float)", asMETHOD(ScriptEntity, set_scale_x), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_scale_y(float)", asMETHOD(ScriptEntity, set_scale_y), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "vec2 get_scale()", asMETHOD(ScriptEntity, get_scale), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_scale_x()", asMETHOD(ScriptEntity, get_scale_x), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_scale_y()", asMETHOD(ScriptEntity, get_scale_y), asCALL_THISCALL);


    }
} // namespace Cobalt

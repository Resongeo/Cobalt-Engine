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
        inline auto GetEngineCtx() -> EngineContext* {
            const auto* as_ctx = asGetActiveContext();
            if (!as_ctx) {
                return nullptr;
            }

            const auto* engine = as_ctx->GetEngine();
            return static_cast<EngineContext*>(engine->GetUserData());
        }

        static auto GetActiveRegistry() -> entt::registry* {
            if (const auto* ctx = GetEngineCtx(); ctx) {
                return &ctx->scene_manager.GetActiveScene()->GetRegistry();
            }

            return nullptr;
        }
    }

    auto TestPrint(String& msg) -> void {
        CORE_INFO("Script: {}", msg);
    }

    auto Vec2DefaultConstructor(void* memory) -> void {
        new(memory)Vec2(0.0f);
    }

    auto Vec2Constructor(const f32 x, const f32 y, void* memory) -> void {
        new(memory)Vec2(x, y);
    }

    auto Vec2ScalarConstructor(const f32 scalar, void* memory) -> void {
        new(memory)Vec2(scalar);
    }

    auto EntityFindByName(const String& name) -> ScriptEntity {
        auto* registry = Utils::GetActiveRegistry();
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

    auto ScriptGlue::RegisterTypes(asIScriptEngine* engine) -> void {
        engine->RegisterObjectType("Entity", sizeof(ScriptEntity), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);

        engine->RegisterObjectType("vec2", sizeof(Vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_ALLFLOATS);
        engine->RegisterObjectProperty("vec2", "float x", asOFFSET(Vec2, x));
        engine->RegisterObjectProperty("vec2", "float y", asOFFSET(Vec2, y));
        engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vec2DefaultConstructor), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(Vec2ScalarConstructor), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(Vec2Constructor), asCALL_CDECL_OBJLAST);
    }

    auto ScriptGlue::RegisterGlobalFunctions(asIScriptEngine* engine) -> void {
        engine->RegisterGlobalFunction("void Print(string& in)", asFUNCTION(TestPrint), asCALL_CDECL);

        engine->SetDefaultNamespace("Scene");
        engine->RegisterGlobalFunction("Entity find_by_name(string& in)", asFUNCTION(EntityFindByName), asCALL_CDECL);
        engine->SetDefaultNamespace("");
    }

    auto ScriptGlue::RegisterEntityAPI(asIScriptEngine* engine) -> void {
        // -- Transform APIs --
        engine->RegisterObjectMethod("Entity", "void set_position(vec2& in)", asMETHOD(ScriptEntity, SetPosition), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_position_x(float)", asMETHOD(ScriptEntity, SetPositionX), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_position_y(float)", asMETHOD(ScriptEntity, SetPositionY), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "vec2 get_position()", asMETHOD(ScriptEntity, GetPosition), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_position_x()", asMETHOD(ScriptEntity, GetPositionX), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_position_y()", asMETHOD(ScriptEntity, GetPositionY), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float set_rotation(float)", asMETHOD(ScriptEntity, SetRotation), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_rotation()", asMETHOD(ScriptEntity, GetRotation), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_scale(vec2& in)", asMETHOD(ScriptEntity, SetScale), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_scale_x(float)", asMETHOD(ScriptEntity, SetScaleX), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "void set_scale_y(float)", asMETHOD(ScriptEntity, SetScaleY), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "vec2 get_scale()", asMETHOD(ScriptEntity, GetScale), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_scale_x()", asMETHOD(ScriptEntity, GetScaleX), asCALL_THISCALL);
        engine->RegisterObjectMethod("Entity", "float get_scale_y()", asMETHOD(ScriptEntity, GetScaleY), asCALL_THISCALL);


    }
} // namespace Cobalt

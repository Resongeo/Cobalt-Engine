// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Scripting/ScriptManager.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Scripting/ScriptEntity.hpp"
#include "Engine/Scripting/ScriptGlue.hpp"

#include <scriptbuilder/scriptbuilder.h>
#include <scripthandle/scripthandle.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptmath/scriptmath.h>
#include <weakref/weakref.h>

#include <format>

namespace Cobalt
{
    auto ScriptManager::Init(EngineContext& ctx) -> bool {
        _engine = asCreateScriptEngine();
        if (_engine == nullptr) {
            return false;
        }

        _context = _engine->CreateContext();
        if (_context == nullptr) {
            return false;
        }

        auto result = 0;
        result = _engine->SetMessageCallback(asMETHOD(ScriptManager, MessageCallback), this, asCALL_THISCALL);
        if (result < 0) {
            return false;
        }

        _engine->SetUserData(&ctx);

        RegisterStdString(_engine);
        RegisterScriptMath(_engine);
        RegisterScriptHandle(_engine);
        RegisterScriptWeakRef(_engine);

        result = _engine->RegisterInterface("IEntity");
        if (result < 0) {
            return false;
        }

        ScriptGlue::RegisterTypes(_engine);
        ScriptGlue::RegisterGlobalFunctions(_engine);
        ScriptGlue::RegisterEntityAPI(_engine);

        return true;
    }

    auto ScriptManager::ShutDown() const -> void {
        _context->Release();
        _engine->ShutDownAndRelease();
    }

    auto ScriptManager::CompileScript(Rc<Script>& script) const -> bool {
        _engine->DiscardModule(script->module.c_str());

        auto result = 0;
        auto builder = CScriptBuilder{};

        result = builder.StartNewModule(_engine, script->module.c_str());
        if (result < 0) {
            script = nullptr;
            return false;
        }

        result = builder.AddSectionFromFile(script->module.c_str());
        if (result < 0) {
            script = nullptr;
            return false;
        }

        result = builder.BuildModule();
        if (result < 0) {
            script = nullptr;
            return false;
        }

        asITypeInfo* type = nullptr;
        const auto module = _engine->GetModule(script->module.c_str(), asGM_ONLY_IF_EXISTS);
        const auto type_count = module->GetObjectTypeCount();
        for (auto i = 0; i < type_count; i++) {
            auto found = false;
            type = module->GetObjectTypeByIndex(i);
            auto interface_count = type->GetInterfaceCount();

            for (auto j = 0; j < interface_count; j++) {
                if (strcmp(type->GetInterface(j)->GetName(), "IEntity") == 0) {
                    found = true;
                    break;
                }
            }

            if (found) {
                script->type_info = type;
                break;
            }
        }

        if (!script->type_info) {
            return false;
        }

        const auto factory_decl = std::format("{} @{}()", script->type_info->GetName(), script->type_info->GetName());
        script->factory_func = script->type_info->GetFactoryByDecl(factory_decl.c_str());
        script->update_func = script->type_info->GetMethodByDecl("void Update(float)");
        script->start_func = script->type_info->GetMethodByDecl("void Start()");

        return true;
    }

    auto ScriptManager::LoadScript(const String& script_path) const -> Rc<Script> {
        auto script = Memory::MakeRc<Script>();

        script->module = script_path;
        if (!CompileScript(script)) {
            return nullptr;
        }

        return script;
    }

    auto ScriptManager::InstantiateScript(EngineContext& ctx, const entt::entity entity, const Rc<Script>& script) const
            -> asIScriptObject* {
        if (!script || !script->factory_func) {
            return nullptr;
        }

        asIScriptObject* instance = nullptr;
        auto result = 0;
        _context->Prepare(script->factory_func);
        result = _context->Execute();

        if (result == asEXECUTION_FINISHED) {
            instance = *static_cast<asIScriptObject**>(_context->GetAddressOfReturnValue());
            instance->AddRef();
        }

        _context->Unprepare();

        if (instance && script->type_info) {
            auto self_prop_idx = -1;
            const auto prop_count = script->type_info->GetPropertyCount();

            for (auto i = 0; i < prop_count; ++i) {
                const char* prop_name = nullptr;

                script->type_info->GetProperty(i, &prop_name);

                if (prop_name && std::strcmp(prop_name, "self") == 0) {
                    self_prop_idx = i;
                    break;
                }
            }

            if (self_prop_idx >= 0) {
                if (auto* self_property = static_cast<ScriptEntity*>(instance->GetAddressOfProperty(self_prop_idx))) {
                    self_property->entity = {entity, &SceneManager::Get().GetActiveScene()->GetRegistry()};
                }
            }
        }

        return instance;
    }

    auto ScriptManager::ExecuteStart(const Rc<Script>& script, asIScriptObject* instance) const -> void {
        if (!instance || !script->start_func) {
            return;
        }

        auto result = 0;
        _context->Prepare(script->start_func);
        _context->SetObject(instance);
        result = _context->Execute();

        if (result == asEXECUTION_EXCEPTION) {
            CORE_ERROR("ScriptManager: Runtime exception: {}", _context->GetExceptionString());
        }

        _context->Unprepare();
    }

    auto ScriptManager::ExecuteUpdate(const Rc<Script>& script, asIScriptObject* instance, const f32 delta_time) const -> void {
        if (!instance || !script->update_func) {
            return;
        }

        auto result = 0;
        _context->Prepare(script->update_func);
        _context->SetArgFloat(0, delta_time);
        _context->SetObject(instance);
        result = _context->Execute();

        if (result == asEXECUTION_EXCEPTION) {
            CORE_ERROR("ScriptManager: Runtime exception: {}", _context->GetExceptionString());
        }

        _context->Unprepare();
    }

    auto ScriptManager::GetEngine() const -> asIScriptEngine* {
        return _engine;
    }

    auto ScriptManager::GetContext() const -> asIScriptContext* {
        return _context;
    }

    auto ScriptManager::Get() -> ScriptManager& {
        static ScriptManager instance;
        return instance;
    }

    auto ScriptManager::MessageCallback(const asSMessageInfo& msg) const -> void {
        if (msg.type == asMSGTYPE_INFORMATION) {
            CORE_INFO("ScriptManager: {} ({}:{}) {}", msg.section, msg.row, msg.col, msg.message);
        }

        if (msg.type == asMSGTYPE_WARNING) {
            CORE_WARN("ScriptManager: {} ({}:{}) {}", msg.section, msg.row, msg.col, msg.message);
        }

        if (msg.type == asMSGTYPE_ERROR) {
            CORE_ERROR("ScriptManager: {} ({}:{}) {}", msg.section, msg.row, msg.col, msg.message);
        }
    }
} // namespace Cobalt

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
#include <weakref/weakref.h>

#include <format>

namespace Cobalt
{
    auto ScriptManager::init(EngineContext& ctx) -> bool {
        m_engine = asCreateScriptEngine();
        if (m_engine == nullptr) {
            return false;
        }

        m_context = m_engine->CreateContext();
        if (m_context == nullptr) {
            return false;
        }

        auto result = 0;
        result = m_engine->SetMessageCallback(asMETHOD(ScriptManager, message_callback), this, asCALL_THISCALL);
        if (result < 0) {
            return false;
        }

        m_engine->SetUserData(&ctx);

        RegisterStdString(m_engine);
        RegisterScriptHandle(m_engine);
        RegisterScriptWeakRef(m_engine);

        result = m_engine->RegisterInterface("IEntity");
        if (result < 0) {
            return false;
        }

        ScriptGlue::register_types(m_engine);
        ScriptGlue::register_global_functions(m_engine);
        ScriptGlue::register_entity_api(m_engine);

        return true;
    }

    auto ScriptManager::destroy() const -> void {
        m_context->Release();
        m_engine->ShutDownAndRelease();
    }

    auto ScriptManager::compile_script(Rc<Script>& script) const -> bool {
        m_engine->DiscardModule(script->module.c_str());

        auto result = 0;
        auto builder = CScriptBuilder{};

        result = builder.StartNewModule(m_engine, script->module.c_str());
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
        const auto module = m_engine->GetModule(script->module.c_str(), asGM_ONLY_IF_EXISTS);
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

    auto ScriptManager::load_script(const String& script_path) const -> Rc<Script> {
        auto script = Memory::make_rc<Script>();

        script->module = script_path;
        if (!compile_script(script)) {
            return nullptr;
        }

        return script;
    }

    auto ScriptManager::instantiate_script(EngineContext& ctx, const entt::entity entity, const Rc<Script>& script) const
            -> asIScriptObject* {
        if (!script || !script->factory_func) {
            return nullptr;
        }

        asIScriptObject* instance = nullptr;
        auto result = 0;
        m_context->Prepare(script->factory_func);
        result = m_context->Execute();

        if (result == asEXECUTION_FINISHED) {
            instance = *static_cast<asIScriptObject**>(m_context->GetAddressOfReturnValue());
            instance->AddRef();
        }

        m_context->Unprepare();

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
                    self_property->entity = {entity, &ctx.scene_manager.get_active_scene()->get_registry()};
                }
            }
        }

        return instance;
    }

    auto ScriptManager::execute_start(const Rc<Script>& script, asIScriptObject* instance) const -> void {
        if (!instance || !script->start_func) {
            return;
        }

        auto result = 0;
        m_context->Prepare(script->start_func);
        m_context->SetObject(instance);
        result = m_context->Execute();

        if (result == asEXECUTION_EXCEPTION) {
            CORE_ERROR("ScriptManager: Runtime exception: {}", m_context->GetExceptionString());
        }

        m_context->Unprepare();
    }

    auto ScriptManager::execute_update(const Rc<Script>& script, asIScriptObject* instance) const -> void {
        if (!instance || !script->update_func) {
            return;
        }

        auto result = 0;
        m_context->Prepare(script->update_func);
        m_context->SetObject(instance);
        result = m_context->Execute();

        if (result == asEXECUTION_EXCEPTION) {
            CORE_ERROR("ScriptManager: Runtime exception: {}", m_context->GetExceptionString());
        }

        m_context->Unprepare();
    }

    auto ScriptManager::get_engine() const -> asIScriptEngine* {
        return m_engine;
    }

    auto ScriptManager::get_context() const -> asIScriptContext* {
        return m_context;
    }

    auto ScriptManager::message_callback(const asSMessageInfo& msg) const -> void {
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

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Logger.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <scripthandle/scripthandle.h>
#include <weakref/weakref.h>
#include <format>

namespace Cobalt
{
    void test_print(String& msg) {
        Logger::warn("Test AngelScript", "{}", msg);
    }

    auto ScriptManager::init() -> bool {
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

        RegisterStdString(m_engine);
        RegisterScriptHandle(m_engine);
        RegisterScriptWeakRef(m_engine);

        result = m_engine->RegisterInterface("IEntity");
        if (result < 0) {
            return false;
        }

        m_engine->RegisterGlobalFunction("void Print(string& in)", asFUNCTION(test_print), asCALL_CDECL);

        return true;
    }

    auto ScriptManager::destroy() const -> void {
        m_context->Release();
        m_engine->ShutDownAndRelease();
    }

    auto ScriptManager::load_script(const String& script_path) const -> Rc<Script> {
        auto script = Memory::make_rc<Script>();

        auto result = 0;
        auto builder = CScriptBuilder{};

        result = builder.StartNewModule(m_engine, script_path.c_str());
        if (result < 0) {
            return nullptr;
        }

        result = builder.AddSectionFromFile(script_path.c_str());
        if (result < 0) {
            return nullptr;
        }

        result = builder.BuildModule();
        if (result < 0) {
            return nullptr;
        }

        script->module = script_path;

        asITypeInfo* type = nullptr;
        const auto module = m_engine->GetModule(script_path.c_str(), asGM_ONLY_IF_EXISTS);
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
            return nullptr;
        }

        const auto factory_decl = std::format("{} @{}()", script->type_info->GetName(), script->type_info->GetName());
        script->factory_func = script->type_info->GetFactoryByDecl(factory_decl.c_str());
        script->update_func = script->type_info->GetMethodByDecl("void Update(float)");
        script->start_func = script->type_info->GetMethodByDecl("void Start()");

        return script;
    }

    auto ScriptManager::instantiate_script(const Rc<Script>& script) const -> void {
        if (!script || !script->factory_func) {
            return;
        }

        auto result = 0;
        m_context->Prepare(script->factory_func);
        result = m_context->Execute();

        if (result == asEXECUTION_FINISHED) {
            script->instance = *static_cast<asIScriptObject**>(m_context->GetAddressOfReturnValue());
            script->instance->AddRef();
        }

        m_context->Unprepare();
    }

    auto ScriptManager::execute_start(const Rc<Script>& script) const -> void {
        if (!script->instance || !script->start_func) {
            return;
        }

        auto result = 0;
        m_context->Prepare(script->start_func);
        m_context->SetObject(script->instance);
        result = m_context->Execute();

        if (result == asEXECUTION_EXCEPTION) {
            Logger::error("Script Manager", "Runtime exception: {}", m_context->GetExceptionString());
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
            Logger::trace(
                "Script Manager", "{} ({}:{}) {}",
                msg.section, msg.row, msg.col, msg.message
            );
        }

        if (msg.type == asMSGTYPE_WARNING) {
            Logger::warn(
                "Script Manager", "{} ({}:{}) {}",
                msg.section, msg.row, msg.col, msg.message
            );
        }

        if (msg.type == asMSGTYPE_ERROR) {
            Logger::error(
                "Script Manager", "{} ({}:{}) {}",
                msg.section, msg.row, msg.col, msg.message
            );
        }
    }
} // namespace Cobalt

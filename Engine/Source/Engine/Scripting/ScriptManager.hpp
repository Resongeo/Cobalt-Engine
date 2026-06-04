// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Scripting/Script.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Fwd.hpp"

#include <angelscript.h>
#include <entt/entity/entity.hpp>

namespace Cobalt
{
    class ScriptManager final
    {
    public:
        auto init(EngineContext& ctx) -> bool;
        auto destroy() const -> void;

        auto load_script(const String& script_path) const -> Rc<Script>;
        auto compile_script(Rc<Script>& script) const -> bool;
        auto instantiate_script(EngineContext& ctx, entt::entity entity, const Rc<Script>& script) const -> asIScriptObject*;
        auto execute_start(const Rc<Script>& script, asIScriptObject* instance) const -> void;
        auto execute_update(const Rc<Script>& script, asIScriptObject* instance) const -> void;

        auto get_engine() const -> asIScriptEngine*;
        auto get_context() const -> asIScriptContext*;

    private:
        auto message_callback(const asSMessageInfo& msg) const -> void;

    private:
        asIScriptEngine* m_engine = nullptr;
        asIScriptContext* m_context = nullptr;
    };
}
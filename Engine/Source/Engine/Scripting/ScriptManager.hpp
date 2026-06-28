// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Scripting/Script.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Fwd.hpp"

#include <angelscript.h>
#include <entt/entity/entity.hpp>

namespace Cobalt
{
    class ScriptManager final
    {
    public:
        auto Init(EngineContext& ctx) -> bool;
        auto ShutDown() const -> void;

        auto LoadScript(const String& script_path) const -> Rc<Script>;
        auto CompileScript(Rc<Script>& script) const -> bool;
        auto InstantiateScript(EngineContext& ctx, entt::entity entity, const Rc<Script>& script) const -> asIScriptObject*;
        auto ExecuteStart(const Rc<Script>& script, asIScriptObject* instance) const -> void;
        auto ExecuteUpdate(const Rc<Script>& script, asIScriptObject* instance, f32 delta_time) const -> void;

        auto GetEngine() const -> asIScriptEngine*;
        auto GetContext() const -> asIScriptContext*;

        static auto Get() -> ScriptManager&;

    private:
        auto MessageCallback(const asSMessageInfo& msg) const -> void;

    private:
        asIScriptEngine* _engine = nullptr;
        asIScriptContext* _context = nullptr;
    };
}
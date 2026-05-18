// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Scripting/Script.hpp"
#include "Engine/Core/Types/Memory.hpp"

#include <angelscript.h>

namespace Cobalt
{
    class ScriptManager final
    {
    public:
        auto init() -> bool;
        auto destroy() const -> void;

        auto load_script(const String& script_path) const -> Rc<Script>;
        auto instantiate_script(const Rc<Script>& script) const -> void;
        auto execute_start(const Rc<Script>& script) const -> void;

        auto get_engine() const -> asIScriptEngine*;
        auto get_context() const -> asIScriptContext*;

    private:
        auto message_callback(const asSMessageInfo& msg) const -> void;

    private:
        asIScriptEngine* m_engine = nullptr;
        asIScriptContext* m_context = nullptr;
    };
}
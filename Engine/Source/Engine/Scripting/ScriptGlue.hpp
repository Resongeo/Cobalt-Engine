// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <angelscript.h>

namespace Cobalt::ScriptGlue
{
    auto RegisterTypes(asIScriptEngine* engine) -> void;
    auto RegisterGlobalFunctions(asIScriptEngine* engine) -> void;
    auto RegisterEntityAPI(asIScriptEngine* engine) -> void;
} // namespace Cobalt::ScriptGlue

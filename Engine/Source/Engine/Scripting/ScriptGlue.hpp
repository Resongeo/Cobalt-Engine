// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <angelscript.h>

namespace Cobalt::ScriptGlue
{
    auto register_types(asIScriptEngine* engine) -> void;
    auto register_global_functions(asIScriptEngine* engine) -> void;
    auto register_entity_api(asIScriptEngine* engine) -> void;
} // namespace Cobalt::ScriptGlue

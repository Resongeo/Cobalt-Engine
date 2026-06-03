// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/UUID.hpp"
#include <angelscript.h>

namespace Cobalt
{
    struct ScriptComponent
    {
        UUID script_id;
        asIScriptObject* instance = nullptr;
    };
} // namespace Cobalt

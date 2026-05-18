// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAsset.hpp"
#include "Engine/Core/Types/Containers.hpp"

#include <angelscript.h>

namespace Cobalt
{
    struct Script final : IAsset
    {
        String module = {};
        asITypeInfo* type_info = nullptr;
        asIScriptObject* instance = nullptr;
        asIScriptFunction* factory_func = nullptr;
        asIScriptFunction* update_func = nullptr;
        asIScriptFunction* start_func = nullptr;
    };
}
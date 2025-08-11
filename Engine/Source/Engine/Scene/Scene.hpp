// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types.hpp"

namespace Cobalt::Engine
{
    struct Scene
    {
        String name = "Default";
        // TODO: Have an EnTT registry

        explicit Scene() = default;
        explicit Scene(const String& name) : name(name) {}
    };
}
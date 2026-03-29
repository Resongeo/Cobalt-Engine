// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"

namespace Cobalt::Engine
{
    struct TagComponent
    {
        String name;
        UUID uuid;

        TagComponent() = default;
        TagComponent(const String& name, const UUID uuid) : name(name), uuid(uuid) {}
        TagComponent(const String& name) : name(name) {
            uuid = UUID::generate();
        }
    };
} // namespace Cobalt::Engine


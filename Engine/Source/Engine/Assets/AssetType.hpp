// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"

namespace Cobalt
{
    enum class AssetType : u8
    {
        None = 0,
        Texture,
        Scene,
        Script,

        SIZE
    };
}

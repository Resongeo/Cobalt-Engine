// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/AssetType.hpp"
#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt
{
    struct AssetMetadata
    {
        Filepath path = {};
        AssetType type = AssetType::None;
    };
} // namespace Cobalt

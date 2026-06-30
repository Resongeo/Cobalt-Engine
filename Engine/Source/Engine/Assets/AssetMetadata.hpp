// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/AssetType.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/UUID.hpp"

namespace Cobalt
{
    struct AssetMetadata
    {
        UUID uuid = {};
        Filepath path = {};
        AssetType type = AssetType::None;
        std::filesystem::file_time_type last_modified = {};
        bool is_memory = false;
    };
} // namespace Cobalt

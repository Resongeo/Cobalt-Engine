// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/AssetMetadata.hpp"
#include "Engine/Assets/IAsset.hpp"
#include "Engine/Core/Types/Memory.hpp"

namespace Cobalt
{
    class IAssetLoader
    {
    public:
        virtual ~IAssetLoader() = default;
        virtual auto load(const AssetMetadata& metadata) -> Rc<IAsset> = 0;
    };
}
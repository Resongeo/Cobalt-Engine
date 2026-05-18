// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/AssetMetadata.hpp"
#include "Engine/Assets/IAsset.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Fwd.hpp"

namespace Cobalt
{
    class IAssetLoader
    {
    public:
        virtual ~IAssetLoader() = default;
        virtual auto load(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> = 0;
    };
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/AssetMetadata.hpp"
#include "Engine/Assets/IAsset.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Fwd.hpp"

namespace Cobalt
{
    class IAssetSerializer
    {
    public:
        virtual ~IAssetSerializer() = default;

        virtual auto deserialize(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> = 0;
        virtual auto serialize(const Rc<IAsset>& asset, const AssetMetadata& metadata) -> bool = 0;
    };
}
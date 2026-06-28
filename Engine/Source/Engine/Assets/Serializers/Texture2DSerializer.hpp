// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAssetSerializer.hpp"

namespace Cobalt
{
    class Texture2DSerializer final : public IAssetSerializer
    {
    public:
        auto Deserialize(const AssetMetadata& metadata) -> Rc<IAsset> override;
        auto Serialize(const Rc<IAsset>& asset, const AssetMetadata& metadata) -> bool override;
    };
}
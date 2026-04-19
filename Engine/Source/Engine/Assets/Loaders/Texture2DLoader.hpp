// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAssetLoader.hpp"

namespace Cobalt
{
    class Texture2DLoader final : public IAssetLoader
    {
    public:
        auto load(const AssetMetadata& metadata) -> Rc<IAsset> override;
    };
}
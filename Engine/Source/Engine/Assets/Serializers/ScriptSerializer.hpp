// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAssetSerializer.hpp"

namespace Cobalt
{
    class ScriptSerializer final : public IAssetSerializer
    {
    public:
        auto deserialize(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> override;
        auto serialize(const Rc<IAsset>& asset, const AssetMetadata& metadata) -> bool override;
    };
}
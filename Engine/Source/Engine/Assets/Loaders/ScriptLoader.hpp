// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAssetLoader.hpp"

namespace Cobalt
{
    class ScriptLoader final : public IAssetLoader
    {
    public:
        auto load(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> override;
    };
}
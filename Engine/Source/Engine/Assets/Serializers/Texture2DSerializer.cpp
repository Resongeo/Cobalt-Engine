// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/Serializers/Texture2DSerializer.hpp"
#include "Engine/Graphics/Texture2D.hpp"

namespace Cobalt
{
    auto Texture2DSerializer::Deserialize(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> {
        auto texture_2d = Memory::MakeRc<Texture2D>();

        if (texture_2d->LoadFromFile(metadata.path)) {
            return texture_2d;
        }

        return nullptr;
    }

    auto Texture2DSerializer::Serialize(const Rc<IAsset>& asset, const AssetMetadata& metadata) -> bool {
        return true;
    }
} // namespace Cobalt

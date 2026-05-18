// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/Loaders/Texture2DLoader.hpp"
#include "Engine/Graphics/Texture2D.hpp"

namespace Cobalt
{
    auto Texture2DLoader::load(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> {
        auto texture_2d = Memory::make_rc<Texture2D>();

        if (texture_2d->load_from_file(metadata.path)) {
            return texture_2d;
        }

        return nullptr;
    }
} // namespace Cobalt

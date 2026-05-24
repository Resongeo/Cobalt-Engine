// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/Serializers/ScriptSerializer.hpp"
#include "Engine/Core/EngineContext.hpp"

namespace Cobalt
{
    auto ScriptSerializer::deserialize(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> {
        // TODO: Load bytecode when implemented
        return ctx.script_manager.load_script(metadata.path.string());
    }

    auto ScriptSerializer::serialize(const Rc<IAsset>& asset, const AssetMetadata& metadata) -> bool {
        return true;
    }
} // namespace Cobalt

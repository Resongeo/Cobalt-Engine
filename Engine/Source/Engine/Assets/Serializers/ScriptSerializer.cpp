// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/Serializers/ScriptSerializer.hpp"
#include "Engine/Scripting/ScriptManager.hpp"

namespace Cobalt
{
    auto ScriptSerializer::Deserialize(const AssetMetadata& metadata) -> Rc<IAsset> {
        // TODO: Load bytecode when implemented
        return ScriptManager::Get().LoadScript(metadata.path.string());
    }

    auto ScriptSerializer::Serialize(const Rc<IAsset>& asset, const AssetMetadata& metadata) -> bool {
        return true;
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Assets/Loaders/ScriptLoader.hpp"

#include "Engine/Core/EngineContext.hpp"

namespace Cobalt
{
    auto ScriptLoader::load(EngineContext& ctx, const AssetMetadata& metadata) -> Rc<IAsset> {
        // TODO: Load bytecode when implemented
        return ctx.script_manager.load_script(metadata.path.string());
    }
} // namespace Cobalt

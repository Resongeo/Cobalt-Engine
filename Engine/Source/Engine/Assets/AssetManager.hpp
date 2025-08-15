// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types.hpp"

namespace Cobalt::Engine
{
    class AssetManager final
    {
    public:
        template<typename T>
        auto get(const UUID id) const -> Shared<T> {
            if (const auto it = m_loaded_assets.find(id); it != m_loaded_assets.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            // TODO: Load asset
            return nullptr;
        }

    private:
        HashMap<UUID, Shared<void>> m_loaded_assets;
    };
}
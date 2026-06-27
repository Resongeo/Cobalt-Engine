// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Assets/IAsset.hpp"
#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt
{
    class Texture2D final : public IAsset
    {
    public:
        ~Texture2D() override;

        auto LoadFromFile(const Filepath& path) -> bool;
        auto CreateWithSize(u32 width, u32 height) -> bool;
        auto BindSlot(u32 slot) const -> void;

        auto GetWidth() const -> u32;
        auto GetHeight() const -> u32;
        auto GetRendererID() const -> u32;

    private:
        u32 _renderer_id = 0;
        u32 _width = 0;
        u32 _height = 0;
    };
}
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

        auto load_from_file(const Filepath& path) -> bool;
        auto bind_slot(u32 slot) const -> void;

        auto get_width() const -> u32;
        auto get_height() const -> u32;
        auto get_renderer_id() const -> u32;

    private:
        u32 m_renderer_id = 0;
        u32 m_width = 0;
        u32 m_height = 0;
    };
}
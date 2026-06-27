// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"

namespace Cobalt
{
    class IndexBuffer final
    {
    public:
        IndexBuffer() = default;
        ~IndexBuffer();

        auto Bind() const -> void;
        auto Unbind() const -> void;

        auto Create(u32 count, const u32* indices) -> void;
        auto GetCount() const -> u32;

    private:
        u32 _renderer_id = 0;
        u32 _count = 0;
    };
} // namespace Cobalt

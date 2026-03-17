// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"

namespace Cobalt::Engine
{
    class IndexBuffer final
    {
    public:
        IndexBuffer() = default;
        ~IndexBuffer();

        auto bind() const -> void;
        auto unbind() const -> void;

        auto create(u32 count, const u32* indices) -> void;
        auto count() const -> u32;

    private:
        u32 m_renderer_id = 0;
        u32 m_count = 0;
    };
}
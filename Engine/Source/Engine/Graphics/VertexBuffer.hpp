// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Graphics/Attributes.hpp"

namespace Cobalt
{
    class VertexBuffer final
    {
    public:
        VertexBuffer() = default;
        ~VertexBuffer();

        auto bind() const -> void;
        auto unbind() const -> void;

        auto create_static(const f32* vertices, u32 size) -> void;
        auto create_dynamic(u32 size) -> void;
        auto set_attribute_layout(const AttributeLayout& layout) -> void;
        auto attribute_layout() -> AttributeLayout&;
        auto copy_data(u32 size, const void* data) const -> void;

    private:
        u32 m_renderer_id = 0;
        AttributeLayout m_attribute_layout = {};
    };
} // namespace Cobalt

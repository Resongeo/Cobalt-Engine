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

        auto Bind() const -> void;
        auto Unbind() const -> void;

        auto CreateStatic(const f32* vertices, u32 size) -> void;
        auto CreateDynamic(u32 size) -> void;
        auto SetAttributeLayout(const AttributeLayout& layout) -> void;
        auto GetAttributeLayout() -> AttributeLayout&;
        auto CopyData(u32 size, const void* data) const -> void;

    private:
        u32 _renderer_id = 0;
        AttributeLayout _attribute_layout = {};
    };
} // namespace Cobalt

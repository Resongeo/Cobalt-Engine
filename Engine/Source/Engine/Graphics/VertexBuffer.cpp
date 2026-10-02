// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/VertexBuffer.hpp"
#include "Engine/Core/Log.hpp"

namespace Cobalt
{
    VertexBuffer::~VertexBuffer() {
        CORE_INFO("Graphics::VertexBuffer: Deleting. ID: {}", _renderer_id);
        _renderer_id = 0;
    }

    auto VertexBuffer::CreateStatic(const f32* vertices, const u32 size) -> void {
        CORE_INFO("Graphics::VertexBuffer: Creating. ID: {}", _renderer_id);
    }

    auto VertexBuffer::CreateDynamic(const u32 size) -> void {
        CORE_INFO("Graphics::VertexBuffer: Creating. ID: {}", _renderer_id);
    }

    auto VertexBuffer::SetAttributeLayout(const AttributeLayout& layout) -> void {
        _attribute_layout = layout;
    }

    auto VertexBuffer::Bind() const -> void {
    }

    auto VertexBuffer::Unbind() const -> void {
    }

    auto VertexBuffer::GetAttributeLayout() -> AttributeLayout& {
        return _attribute_layout;
    }

    auto VertexBuffer::CopyData(const u32 size, const void* data) const -> void {
    }
} // namespace Cobalt

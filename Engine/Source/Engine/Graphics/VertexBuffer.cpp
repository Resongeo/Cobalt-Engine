// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/VertexBuffer.hpp"
#include "Engine/Core/Log.hpp"

#include <glad/gl.h>

namespace Cobalt
{
    VertexBuffer::~VertexBuffer() {
        CORE_INFO("Graphics::VertexBuffer: Deleting. ID: {}", _renderer_id);

        glDeleteBuffers(1, &_renderer_id);
        _renderer_id = 0;
    }

    auto VertexBuffer::CreateStatic(const f32* vertices, const u32 size) -> void {
        CORE_INFO("Graphics::VertexBuffer: Creating. ID: {}", _renderer_id);

        glGenBuffers(1, &_renderer_id);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    auto VertexBuffer::CreateDynamic(const u32 size) -> void {
        CORE_INFO("Graphics::VertexBuffer: Creating. ID: {}", _renderer_id);

        glGenBuffers(1, &_renderer_id);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    auto VertexBuffer::SetAttributeLayout(const AttributeLayout& layout) -> void {
        _attribute_layout = layout;
    }

    auto VertexBuffer::Bind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, _renderer_id);
    }

    auto VertexBuffer::Unbind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    auto VertexBuffer::GetAttributeLayout() -> AttributeLayout& {
        return _attribute_layout;
    }

    auto VertexBuffer::CopyData(const u32 size, const void* data) const -> void {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
} // namespace Cobalt

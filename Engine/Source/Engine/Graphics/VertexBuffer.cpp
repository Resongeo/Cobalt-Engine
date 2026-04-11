// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/VertexBuffer.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/gl.h>

namespace Cobalt::Engine
{
    VertexBuffer::~VertexBuffer() {
        Logger::trace("Engine::Graphics::VertexBuffer", "Deleting ID: {}", m_renderer_id);
        glDeleteBuffers(1, &m_renderer_id);
        m_renderer_id = 0;
    }

    auto VertexBuffer::create_static(const f32* vertices, const u32 size) -> void {
        glGenBuffers(1, &m_renderer_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        Logger::trace("Engine::Graphics::VertexBuffer", "Created ID: {}", m_renderer_id);
    }

    auto VertexBuffer::create_dynamic(const u32 size) -> void {
        glGenBuffers(1, &m_renderer_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        Logger::trace("Engine::Graphics::VertexBuffer", "Created ID: {}", m_renderer_id);
    }

    auto VertexBuffer::set_attribute_layout(const AttributeLayout& layout) -> void {
        m_attribute_layout = layout;
    }

    auto VertexBuffer::bind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    }

    auto VertexBuffer::unbind() const -> void {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    auto VertexBuffer::attribute_layout() -> AttributeLayout& {
        return m_attribute_layout;
    }

    auto VertexBuffer::copy_data(const u32 size, const void* data) -> void {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
} // namespace Cobalt::Engine

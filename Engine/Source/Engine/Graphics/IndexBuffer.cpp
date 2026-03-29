// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/IndexBuffer.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/gl.h>

namespace Cobalt::Engine
{
    IndexBuffer::~IndexBuffer() {
        Logger::trace("index buffer", "Deleting ID: {}", m_renderer_id);
        glDeleteBuffers(1, &m_renderer_id);
        m_renderer_id = 0;
    }

    auto IndexBuffer::bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    }

    auto IndexBuffer::unbind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    auto IndexBuffer::create(const u32 count, const u32* indices) -> void {
        m_count = count;

        glGenBuffers(1, &m_renderer_id);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(u32), indices, GL_STATIC_DRAW);
        Logger::trace("index buffer", "Created ID: {}", m_renderer_id);
    }

    auto IndexBuffer::count() const -> u32 {
        return m_count;
    }
} // namespace Cobalt::Engine

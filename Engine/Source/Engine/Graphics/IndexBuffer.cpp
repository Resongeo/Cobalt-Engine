// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/IndexBuffer.hpp"
#include "Engine/Core/Log.hpp"

#include <glad/gl.h>

namespace Cobalt
{
    IndexBuffer::~IndexBuffer() {
        CORE_INFO("Graphics::IndexBuffer: Deleting. ID: {}", _renderer_id);

        glDeleteBuffers(1, &_renderer_id);
        _renderer_id = 0;
    }

    auto IndexBuffer::Bind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderer_id);
    }

    auto IndexBuffer::Unbind() const -> void {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    auto IndexBuffer::Create(const u32 count, const u32* indices) -> void {
        CORE_INFO("Graphics::IndexBuffer: Creating. ID: {}", _renderer_id);

        _count = count;

        glGenBuffers(1, &_renderer_id);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(u32), indices, GL_STATIC_DRAW);
    }

    auto IndexBuffer::GetCount() const -> u32 {
        return _count;
    }
} // namespace Cobalt

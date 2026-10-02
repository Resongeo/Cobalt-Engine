// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/VertexArray.hpp"
#include "Engine/Core/Log.hpp"

namespace Cobalt
{
    VertexArray::~VertexArray() {
    }

    auto VertexArray::Create() -> void {
    }

    auto VertexArray::Bind() const -> void {
    }

    auto VertexArray::Unbind() const -> void {
    }

    auto VertexArray::AddVertexBuffer(const Rc<VertexBuffer>& vertex_buffer) -> void {

    }

    auto VertexArray::SetIndexBuffer(const Rc<IndexBuffer>& buffer) -> void {
        Bind();
        buffer->Bind();

        _index_buffer = buffer;
    }

    auto VertexArray::GetVertexBuffers() const -> const Vector<Rc<VertexBuffer>>& {
        return _vertex_buffers;
    }

    auto VertexArray::GetIndexBuffer() const -> const Rc<IndexBuffer>& {
        return _index_buffer;
    }
} // namespace Cobalt

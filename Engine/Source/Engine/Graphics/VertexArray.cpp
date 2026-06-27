// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/VertexArray.hpp"
#include "Engine/Core/Log.hpp"

#include <glad/gl.h>

namespace Cobalt
{
    VertexArray::~VertexArray() {
        CORE_INFO("Graphics::VertexArray: Deleting. ID: {}", _renderer_id);

        glDeleteVertexArrays(1, &_renderer_id);
        _renderer_id = 0;
    }

    auto VertexArray::Create() -> void {
        CORE_INFO("Graphics::VertexArray: Deleting. ID: {}", _renderer_id);

        glGenVertexArrays(1, &_renderer_id);
        Bind();
    }

    auto VertexArray::Bind() const -> void {
        glBindVertexArray(_renderer_id);
    }

    auto VertexArray::Unbind() const -> void {
        glBindVertexArray(0);
    }

    auto VertexArray::AddVertexBuffer(const Rc<VertexBuffer>& vertex_buffer) -> void {
        Bind();
        vertex_buffer->Bind();

        auto index = 0;
        for (auto& layout = vertex_buffer->GetAttributeLayout(); auto& attribute : layout) {
            switch (attribute.type) {
                case AttributeDataType::Bool:
                case AttributeDataType::Int:
                case AttributeDataType::Int2:
                case AttributeDataType::Int3:
                case AttributeDataType::Int4: {
                    glEnableVertexAttribArray(index);
                    glVertexAttribIPointer(index, attribute.GetElementCount(), GL_INT, layout.GetStride(),
                                           (const void*)attribute.offset);
                    break;
                }
                default: {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index, attribute.GetElementCount(), GL_FLOAT, GL_FALSE, layout.GetStride(),
                                          (const void*)attribute.offset);
                    break;
                }
            }

            index++;
        }

        _vertex_buffers.push_back(vertex_buffer);
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

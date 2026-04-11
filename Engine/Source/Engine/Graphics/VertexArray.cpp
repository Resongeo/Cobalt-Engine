// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/VertexArray.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/gl.h>

namespace Cobalt::Engine
{
    VertexArray::~VertexArray() {
        Logger::trace("Engine::Graphics::VertexArray", "Deleting ID: {}", m_renderer_id);
        glDeleteVertexArrays(1, &m_renderer_id);
        m_renderer_id = 0;
    }

    auto VertexArray::create() -> void {
        glGenVertexArrays(1, &m_renderer_id);
        bind();
        Logger::trace("Engine::Graphics::VertexArray", "Created ID: {}", m_renderer_id);
    }

    auto VertexArray::bind() const -> void {
        glBindVertexArray(m_renderer_id);
    }

    auto VertexArray::unbind() const -> void {
        glBindVertexArray(0);
    }

    auto VertexArray::add_vertex_buffer(const Rc<VertexBuffer>& vertex_buffer) -> void {
        bind();
        vertex_buffer->bind();

        auto index = 0;
        for (auto& layout = vertex_buffer->attribute_layout(); auto& attribute : layout) {
            switch (attribute.type) {
                case AttributeDataType::Bool:
                case AttributeDataType::Int:
                case AttributeDataType::Int2:
                case AttributeDataType::Int3:
                case AttributeDataType::Int4: {
                    glEnableVertexAttribArray(index);
                    glVertexAttribIPointer(index, attribute.element_count(), GL_INT, layout.stride(),
                                           (const void*)attribute.offset);
                    break;
                }
                default: {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index, attribute.element_count(), GL_FLOAT, GL_FALSE, layout.stride(),
                                          (const void*)attribute.offset);
                    break;
                }
            }

            index++;
        }

        m_vertex_buffers.push_back(vertex_buffer);
    }

    auto VertexArray::set_index_buffer(const Rc<IndexBuffer>& index_buffer) -> void {
        bind();
        index_buffer->bind();

        m_index_buffer = index_buffer;
    }

    auto VertexArray::vertex_buffers() const -> const Vector<Rc<VertexBuffer>>& {
        return m_vertex_buffers;
    }

    auto VertexArray::index_buffer() const -> const Rc<IndexBuffer>& {
        return m_index_buffer;
    }
} // namespace Cobalt::Engine

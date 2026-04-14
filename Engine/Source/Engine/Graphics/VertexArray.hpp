// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Graphics/IndexBuffer.hpp"
#include "Engine/Graphics/VertexBuffer.hpp"

namespace Cobalt
{
    class VertexArray final
    {
    public:
        VertexArray() = default;
        ~VertexArray();

        auto bind() const -> void;
        auto unbind() const -> void;

        auto create() -> void;
        auto add_vertex_buffer(const Rc<VertexBuffer>& vertex_buffer) -> void;
        auto set_index_buffer(const Rc<IndexBuffer>& index_buffer) -> void;
        auto vertex_buffers() const -> const Vector<Rc<VertexBuffer>>&;
        auto index_buffer() const -> const Rc<IndexBuffer>&;

    private:
        u32 m_renderer_id = 0;
        Rc<IndexBuffer> m_index_buffer = {};
        Vector<Rc<VertexBuffer>> m_vertex_buffers = {};
    };
} // namespace Cobalt

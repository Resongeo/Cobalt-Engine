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

        auto Bind() const -> void;
        auto Unbind() const -> void;

        auto Create() -> void;
        auto AddVertexBuffer(const Rc<VertexBuffer>& vertex_buffer) -> void;
        auto SetIndexBuffer(const Rc<IndexBuffer>& buffer) -> void;
        auto GetVertexBuffers() const -> const Vector<Rc<VertexBuffer>>&;
        auto GetIndexBuffer() const -> const Rc<IndexBuffer>&;

    private:
        u32 _renderer_id = 0;
        Rc<IndexBuffer> _index_buffer = {};
        Vector<Rc<VertexBuffer>> _vertex_buffers = {};
    };
} // namespace Cobalt

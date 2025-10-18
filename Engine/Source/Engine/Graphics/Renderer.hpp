// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Graphics/Quad.hpp"
#include "Engine/Graphics/Shader.hpp"
#include "Engine/Graphics/VertexArray.hpp"
#include "Engine/Graphics/VertexBuffer.hpp"
#include "Engine/Graphics/Camera.hpp"

namespace Cobalt::Engine
{
    class Renderer final
    {
    public:
        auto init(u32 max_quads, const Rc<Shader>& shader) -> void;
        auto begin_frame(Camera& camera) -> void;
        auto submit_quad(const Vec3& pos, const Vec2& scale, const Vec4& color) -> void;
        auto submit_quad(const QuadDrawCommand& draw_command) -> void;
        auto end_frame() -> void;
        auto set_viewport_size(const Vec<2, i32>& size) -> void;

    private:
        auto _start_batch() -> void;
        auto _flush_batch() -> void;
        auto _is_batch_full() const -> bool;

    private:
        u32 m_max_quads = 0;
        u32 m_quad_index_count = 0;
        Rc<Shader> m_default_shader = {};
        Rc<VertexArray> m_vertex_array = {};
        Rc<VertexBuffer> m_vertex_buffer = {};
        QuadVertexData* m_vertex_buffer_base = nullptr;
        QuadVertexData* m_vertex_buffer_ptr = nullptr;
        Vec<2, u32> m_viewport_size = {};
        Vector<QuadDrawCommand> m_draw_commands = {};
    };
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Graphics/Renderer.hpp"

#include <glad/gl.h>

namespace Cobalt::Engine
{
    auto Renderer::init(const u32 max_quads, const Rc<Shader>& shader) -> void {
        m_max_quads = max_quads;
        m_draw_commands.reserve(m_max_quads);
        m_viewport_size = {1280, 720};
        // TODO: Temp
        m_default_shader = shader;

        const auto max_vertices = m_max_quads * 4;
        const auto max_indices = m_max_quads * 6;

        m_vertex_buffer_base = new QuadVertexData[max_vertices];

        const auto indices = new u32[max_indices];

        u32 offset = 0;
        for (usize i = 0; i < max_indices; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        m_vertex_array = Memory::make_rc<VertexArray>();
        m_vertex_buffer = Memory::make_rc<VertexBuffer>();
        auto index_buffer = Memory::make_rc<IndexBuffer>();

        m_vertex_array->create();
        m_vertex_buffer->create_dynamic(max_vertices * sizeof(QuadVertexData));
        index_buffer->create(max_indices, indices);

        auto attribute_layout = AttributeLayout();
        attribute_layout.create({
            AttributeDataType::Float2, // a_Position
            AttributeDataType::Float4, // a_Color
        });

        m_vertex_buffer->set_attribute_layout(attribute_layout);
        m_vertex_array->add_vertex_buffer(m_vertex_buffer);
        m_vertex_array->set_index_buffer(index_buffer);

        delete[] indices;
    }

    auto Renderer::begin_frame() -> void {
        // TODO: Reset renderer stats
        m_draw_commands.clear();

        glViewport(0, 0, m_viewport_size.x, m_viewport_size.y);

        // TODO: Get clear color from style config
        glClearColor(0.113, 0.113, 0.125, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        //////////////////
        // -- Camera -- //
        //////////////////

        // -- View --
        auto transform = Mat4(1.0f);
        transform = glm::translate(transform, {0, 0, -1});
        auto view = glm::inverse(transform);

        // -- Projection --
        constexpr f32 ar = 1280.0f / 720.0f;
        constexpr f32 size = 2.0f;
        constexpr auto left = -(ar / 2.0f) * size;
        constexpr auto right = (ar / 2.0f) * size;
        constexpr auto bottom = -size / 2.0f;
        constexpr auto top = size / 2.0f;

        const auto proj = glm::ortho(
            left, right, bottom, top, -1.0f, 1.0f
        );

        // -- Combined --
        auto view_proj = proj * view;

        m_default_shader->bind();
        m_default_shader->set_mat4("u_ViewProjection", view_proj);

        _start_batch();
    }

    auto Renderer::submit_quad(const Vec3& pos, const Vec2& scale, const Vec4& color) -> void {
        if (_is_batch_full()) {
            _flush_batch();
            _start_batch();
        }

        const Vec2 offset{ scale.x * 0.5f, scale.y * 0.5f };

        m_vertex_buffer_ptr->position = { pos.x - offset.x, pos.y - offset.y };
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr++;

        m_vertex_buffer_ptr->position = { pos.x + offset.x, pos.y - offset.y };
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr++;

        m_vertex_buffer_ptr->position = { pos.x + offset.x, pos.y + offset.y };
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr++;

        m_vertex_buffer_ptr->position = { pos.x - offset.x, pos.y + offset.y };
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr++;

        m_quad_index_count += 6;
    }

    auto Renderer::submit_quad(const QuadDrawCommand& draw_command) -> void {
        if (_is_batch_full()) {
            _flush_batch();
            m_draw_commands.clear();
        }

        m_draw_commands.push_back(draw_command);
    }

    auto Renderer::end_frame() -> void {
        _flush_batch();
        //m_default_shader->unbind();
    }

    auto Renderer::_flush_batch() -> void {
        uint32_t data_size = (uint8_t*)m_vertex_buffer_ptr - (uint8_t*)m_vertex_buffer_base;
		m_vertex_buffer->copy_data(data_size, m_vertex_buffer_base);

        u32 count = m_quad_index_count == 0 ? m_vertex_array->index_buffer()->count() : m_quad_index_count;

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    auto Renderer::_is_batch_full() const -> bool {
        return m_draw_commands.size() >= m_max_quads;
    }

    auto Renderer::destroy() -> void {

    }

    auto Renderer::_start_batch() -> void {
        m_quad_index_count = 0;
        m_vertex_buffer_ptr = m_vertex_buffer_base;
    }
}

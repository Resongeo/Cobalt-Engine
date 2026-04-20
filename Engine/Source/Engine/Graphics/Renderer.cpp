// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/gl.h>

namespace Cobalt
{
    constexpr Vec4 VERTEX_POSITIONS[4] = {
        {-0.5, -0.5, 0.0, 1.0},
        {0.5, -0.5, 0.0, 1.0},
        {0.5, 0.5, 0.0, 1.0},
        {-0.5, 0.5, 0.0, 1.0},
    };

    constexpr Vec2 TEXTURE_COORDS[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
    };

    auto Renderer::init(const u32 max_quads, const Filepath& base_assets_path) -> void {
        m_max_quads = max_quads;

        m_default_shader = Memory::make_rc<Shader>();
        const auto shaders_path = base_assets_path / "Shaders";
        const auto vertex_path = shaders_path / "DefaultQuad.vert";
        const auto fragment_path = shaders_path / "DefaultQuad.frag";
        const auto vertex_path_str = vertex_path.string();
        const auto fragment_path_str = fragment_path.string();
        if (auto result = m_default_shader->create_from_file(vertex_path_str.c_str(), fragment_path_str.c_str());
            !result) {
            Logger::error("Engine::Shader", "Failed to create default quad shader.");
            result = m_default_shader->create_fallback();
            if (!result) {
                Logger::fatal("Engine::Shader", "Failed to create fallback default quad shader.");
                std::exit(1);
            }
        }

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
        const auto index_buffer = Memory::make_rc<IndexBuffer>();

        m_vertex_array->create();
        m_vertex_buffer->create_dynamic(max_vertices * sizeof(QuadVertexData));
        index_buffer->create(max_indices, indices);

        auto attribute_layout = AttributeLayout();
        attribute_layout.create({
                AttributeDataType::Float2, // a_Position
                AttributeDataType::Float2, // a_TexCoords
                AttributeDataType::Float4, // a_Color
                AttributeDataType::Float,  // a_TexIndex
        });

        m_vertex_buffer->set_attribute_layout(attribute_layout);
        m_vertex_array->add_vertex_buffer(m_vertex_buffer);
        m_vertex_array->set_index_buffer(index_buffer);

        auto texture_samplers = Array<i32, MAX_TEXTURES>{};
        for (auto i = 0; i < MAX_TEXTURES; i++) {
            texture_samplers[i] = i;
        }

        m_default_shader->bind();
        m_default_shader->set_int_array("u_TextureSlots", texture_samplers.data(), texture_samplers.size());

        m_default_texture = Memory::make_rc<Texture2D>();
        m_default_texture->create_with_size(1, 1);
        m_texture_slots[0] = m_default_texture;

        delete[] indices;
    }

    auto Renderer::begin_frame(Camera& camera) -> void {
        // TODO: Reset renderer stats

        const auto col = camera.clear_color;
        const auto view_proj = camera.view_projection(m_viewport_size);

        m_default_shader->bind();
        m_default_shader->set_mat4("u_ViewProjection", view_proj);

        glViewport(0, 0, m_viewport_size.x, m_viewport_size.y);
        glClearColor(col.r, col.g, col.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        start_batch();
    }

    auto Renderer::submit_quad(const Vec3& pos, const Vec2& scale, const Vec4& color) -> void {
        if (is_batch_full()) {
            flush_batch();
            start_batch();
        }

        const Vec2 offset{scale.x * 0.5f, scale.y * 0.5f};

        m_vertex_buffer_ptr->position = {pos.x - offset.x, pos.y - offset.y};
        m_vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[0];
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr->tex_index = 0.0f;
        m_vertex_buffer_ptr++;

        m_vertex_buffer_ptr->position = {pos.x + offset.x, pos.y - offset.y};
        m_vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[1];
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr->tex_index = 0.0f;
        m_vertex_buffer_ptr++;

        m_vertex_buffer_ptr->position = {pos.x + offset.x, pos.y + offset.y};
        m_vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[2];
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr->tex_index = 0.0f;
        m_vertex_buffer_ptr++;

        m_vertex_buffer_ptr->position = {pos.x - offset.x, pos.y + offset.y};
        m_vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[3];
        m_vertex_buffer_ptr->color = color;
        m_vertex_buffer_ptr->tex_index = 0.0f;
        m_vertex_buffer_ptr++;

        m_quad_index_count += 6;
    }

    auto Renderer::submit_quad(const Vec3& pos, const Vec2& scale, const f32 rotation, const Vec4& color) -> void {
        if (is_batch_full()) {
            flush_batch();
            start_batch();
        }

        const auto transform = glm::translate(Mat4(1), pos) * glm::rotate(Mat4(1), glm::radians(rotation), {0, 0, 1}) *
                glm::scale(Mat4(1), {scale.x, scale.y, 1});

        for (auto i = 0; i < 4; i++) {
            m_vertex_buffer_ptr->position = transform * VERTEX_POSITIONS[i];
            m_vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[i];
            m_vertex_buffer_ptr->color = color;
            m_vertex_buffer_ptr->tex_index = 0.0f;
            m_vertex_buffer_ptr++;
        }

        m_quad_index_count += 6;
    }

    auto Renderer::submit_quad(const Vec3& pos, const Vec2& scale, f32 rotation, const Vec4& color,
                               const Rc<Texture2D>& texture) -> void {
        if (is_batch_full()) {
            flush_batch();
            start_batch();
        }

        auto index = 0;

        if (texture) {
            for (auto i = 0; i < m_texture_index; i++) {
                if (m_texture_slots[i]->get_renderer_id() == texture->get_renderer_id()) {
                    index = i;
                    break;
                }
            }

            if (index == 0) {
                m_texture_slots[m_texture_index] = texture;
                index = m_texture_index;
                m_texture_index++;
            }
        }

        const auto transform = glm::translate(Mat4(1), pos) * glm::rotate(Mat4(1), glm::radians(rotation), {0, 0, 1}) *
                glm::scale(Mat4(1), {scale.x, scale.y, 1});

        for (auto i = 0; i < 4; i++) {
            m_vertex_buffer_ptr->position = transform * VERTEX_POSITIONS[i];
            m_vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[i];
            m_vertex_buffer_ptr->color = color;
            m_vertex_buffer_ptr->tex_index = static_cast<f32>(index);
            m_vertex_buffer_ptr++;
        }

        m_quad_index_count += 6;
    }

    auto Renderer::end_frame() const -> void {
        flush_batch();
    }

    auto Renderer::set_viewport_size(const Vec<2, i32>& size) -> void {
        m_viewport_size = size;
    }

    auto Renderer::flush_batch() const -> void {
        const u32 data_size =
                reinterpret_cast<uint8_t*>(m_vertex_buffer_ptr) - reinterpret_cast<uint8_t*>(m_vertex_buffer_base);
        m_vertex_buffer->copy_data(data_size, m_vertex_buffer_base);

        for (auto i = 0; i < m_texture_index; i++) {
            m_texture_slots[i]->bind_slot(i);
        }

        const u32 count = m_quad_index_count == 0 ? m_vertex_array->index_buffer()->count() : m_quad_index_count;

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    auto Renderer::is_batch_full() const -> bool {
        return m_quad_index_count >= m_max_quads * 6 || m_texture_index >= MAX_TEXTURES;
    }

    auto Renderer::start_batch() -> void {
        m_quad_index_count = 0;
        m_vertex_buffer_ptr = m_vertex_buffer_base;
        m_texture_index = 1;
    }
} // namespace Cobalt

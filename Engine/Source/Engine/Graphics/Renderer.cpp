// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Core/Log.hpp"

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

    auto Renderer::Init(const u32 max_quads, const Filepath& base_assets_path) -> void {
        _max_quads = max_quads;

        _default_shader = Memory::MakeRc<Shader>();
        const auto shaders_path = base_assets_path / "Shaders";
        const auto vertex_path = shaders_path / "DefaultQuad.vert";
        const auto fragment_path = shaders_path / "DefaultQuad.frag";
        const auto vertex_path_str = vertex_path.string();
        const auto fragment_path_str = fragment_path.string();
        if (auto result = _default_shader->CreateFromFile(vertex_path_str.c_str(), fragment_path_str.c_str());
            !result) {
            CORE_ERROR("Graphics::Shader: Failed to Create default quad shader!");
            result = _default_shader->CreateFallback();
            if (!result) {
            CORE_ERROR("Graphics::Shader: Failed to Create fallback quad shader!");
                std::exit(1);
            }
        }

        const auto max_vertices = _max_quads * 4;
        const auto max_indices = _max_quads * 6;

        _vertex_buffer_base = new QuadVertexData[max_vertices];

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

        _vertex_array = Memory::MakeRc<VertexArray>();
        _vertex_buffer = Memory::MakeRc<VertexBuffer>();
        const auto index_buffer = Memory::MakeRc<IndexBuffer>();

        _vertex_array->Create();
        _vertex_buffer->CreateDynamic(max_vertices * sizeof(QuadVertexData));
        index_buffer->Create(max_indices, indices);

        auto attribute_layout = AttributeLayout();
        attribute_layout.Create({
                AttributeDataType::Float2, // a_Position
                AttributeDataType::Float2, // a_TexCoords
                AttributeDataType::Float4, // a_Color
                AttributeDataType::Float,  // a_TexIndex
        });

        _vertex_buffer->SetAttributeLayout(attribute_layout);
        _vertex_array->AddVertexBuffer(_vertex_buffer);
        _vertex_array->SetIndexBuffer(index_buffer);

        auto texture_samplers = Array<i32, MAX_TEXTURES>{};
        for (auto i = 0; i < MAX_TEXTURES; i++) {
            texture_samplers[i] = i;
        }

        _default_shader->Bind();
        _default_shader->SetIntArray("u_TextureSlots", texture_samplers.data(), texture_samplers.size());

        _default_texture = Memory::MakeRc<Texture2D>();
        _default_texture->CreateWithSize(1, 1);
        _texture_slots[0] = _default_texture;

        delete[] indices;
    }

    auto Renderer::BeginFrame(Camera& camera) -> void {
        // TODO: Reset renderer stats

        const auto col = camera.clear_color;
        const auto view_proj = camera.GetViewProjection(_viewport_size);

        _default_shader->Bind();
        _default_shader->SetMat4("u_ViewProjection", view_proj);

        glViewport(0, 0, _viewport_size.x, _viewport_size.y);
        glClearColor(col.r, col.g, col.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        StartBatch();
    }

    auto Renderer::SubmitQuad(const Vec3& pos, const Vec2& scale, const Vec4& color) -> void {
        if (IsBatchFull()) {
            FlushBatch();
            StartBatch();
        }

        const Vec2 offset{scale.x * 0.5f, scale.y * 0.5f};

        _vertex_buffer_ptr->position = {pos.x - offset.x, pos.y - offset.y};
        _vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[0];
        _vertex_buffer_ptr->color = color;
        _vertex_buffer_ptr->tex_index = 0.0f;
        _vertex_buffer_ptr++;

        _vertex_buffer_ptr->position = {pos.x + offset.x, pos.y - offset.y};
        _vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[1];
        _vertex_buffer_ptr->color = color;
        _vertex_buffer_ptr->tex_index = 0.0f;
        _vertex_buffer_ptr++;

        _vertex_buffer_ptr->position = {pos.x + offset.x, pos.y + offset.y};
        _vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[2];
        _vertex_buffer_ptr->color = color;
        _vertex_buffer_ptr->tex_index = 0.0f;
        _vertex_buffer_ptr++;

        _vertex_buffer_ptr->position = {pos.x - offset.x, pos.y + offset.y};
        _vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[3];
        _vertex_buffer_ptr->color = color;
        _vertex_buffer_ptr->tex_index = 0.0f;
        _vertex_buffer_ptr++;

        _quad_index_count += 6;
    }

    auto Renderer::SubmitQuad(const Vec3& pos, const Vec2& scale, const f32 rotation, const Vec4& color) -> void {
        if (IsBatchFull()) {
            FlushBatch();
            StartBatch();
        }

        const auto transform = glm::translate(Mat4(1), pos) * glm::rotate(Mat4(1), glm::radians(rotation), {0, 0, 1}) *
                glm::scale(Mat4(1), {scale.x, scale.y, 1});

        for (auto i = 0; i < 4; i++) {
            _vertex_buffer_ptr->position = transform * VERTEX_POSITIONS[i];
            _vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[i];
            _vertex_buffer_ptr->color = color;
            _vertex_buffer_ptr->tex_index = 0.0f;
            _vertex_buffer_ptr++;
        }

        _quad_index_count += 6;
    }

    auto Renderer::SubmitQuad(const Vec3& pos, const Vec2& scale, f32 rotation, const Vec4& color,
                               const Rc<Texture2D>& texture) -> void {
        if (IsBatchFull()) {
            FlushBatch();
            StartBatch();
        }

        auto index = 0;

        if (texture) {
            for (auto i = 0; i < _texture_index; i++) {
                if (_texture_slots[i]->GetRendererID() == texture->GetRendererID()) {
                    index = i;
                    break;
                }
            }

            if (index == 0) {
                _texture_slots[_texture_index] = texture;
                index = _texture_index;
                _texture_index++;
            }
        }

        const auto transform = glm::translate(Mat4(1), pos) * glm::rotate(Mat4(1), glm::radians(rotation), {0, 0, 1}) *
                glm::scale(Mat4(1), {scale.x, scale.y, 1});

        for (auto i = 0; i < 4; i++) {
            _vertex_buffer_ptr->position = transform * VERTEX_POSITIONS[i];
            _vertex_buffer_ptr->tex_coords = TEXTURE_COORDS[i];
            _vertex_buffer_ptr->color = color;
            _vertex_buffer_ptr->tex_index = static_cast<f32>(index);
            _vertex_buffer_ptr++;
        }

        _quad_index_count += 6;
    }

    auto Renderer::EndFrame() const -> void {
        FlushBatch();
    }

    auto Renderer::SetViewportSize(const Vec<2, i32>& size) -> void {
        _viewport_size = size;
    }

    auto Renderer::FlushBatch() const -> void {
        const u32 data_size =
                reinterpret_cast<uint8_t*>(_vertex_buffer_ptr) - reinterpret_cast<uint8_t*>(_vertex_buffer_base);
        _vertex_buffer->CopyData(data_size, _vertex_buffer_base);

        for (auto i = 0; i < _texture_index; i++) {
            _texture_slots[i]->BindSlot(i);
        }

        const u32 count = _quad_index_count == 0 ? _vertex_array->GetIndexBuffer()->GetCount() : _quad_index_count;

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    auto Renderer::IsBatchFull() const -> bool {
        return _quad_index_count >= _max_quads * 6 || _texture_index >= MAX_TEXTURES;
    }

    auto Renderer::StartBatch() -> void {
        _quad_index_count = 0;
        _vertex_buffer_ptr = _vertex_buffer_base;
        _texture_index = 1;
    }
} // namespace Cobalt

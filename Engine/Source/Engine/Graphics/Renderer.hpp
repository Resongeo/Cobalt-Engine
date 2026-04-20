// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Engine/Graphics/Quad.hpp"
#include "Engine/Graphics/Shader.hpp"
#include "Engine/Graphics/VertexArray.hpp"
#include "Engine/Graphics/VertexBuffer.hpp"
#include "Engine/Graphics/Texture2D.hpp"

namespace Cobalt
{
    class Renderer final
    {
    public:
        auto init(u32 max_quads, const Filepath& base_assets_path) -> void;
        auto begin_frame(Camera& camera) -> void;
        auto submit_quad(const Vec3& pos, const Vec2& scale, const Vec4& color) -> void;
        auto submit_quad(const Vec3& pos, const Vec2& scale, f32 rotation, const Vec4& color) -> void;
        auto submit_quad(const Vec3& pos, const Vec2& scale, f32 rotation, const Vec4& color, const Rc<Texture2D>& texture) -> void;
        auto end_frame() const -> void;
        auto set_viewport_size(const Vec<2, i32>& size) -> void;

    private:
        auto start_batch() -> void;
        auto flush_batch() const -> void;
        auto is_batch_full() const -> bool;

    private:
        static constexpr int MAX_TEXTURES = 32;

        u32 m_max_quads = 0;
        u32 m_quad_index_count = 0;
        Rc<Shader> m_default_shader = {};
        Rc<VertexArray> m_vertex_array = {};
        Rc<VertexBuffer> m_vertex_buffer = {};
        Rc<Texture2D> m_default_texture = {};
        u32 m_texture_index = 1;
        Array<Rc<Texture2D>, MAX_TEXTURES> m_texture_slots = {};
        QuadVertexData* m_vertex_buffer_base = nullptr;
        QuadVertexData* m_vertex_buffer_ptr = nullptr;
        Vec<2, u32> m_viewport_size = {};
    };
} // namespace Cobalt

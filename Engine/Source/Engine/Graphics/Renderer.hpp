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
        auto Init(u32 max_quads, const Filepath& base_assets_path) -> void;
        auto BeginFrame(Camera& camera) -> void;
        auto SubmitQuad(const Vec3& pos, const Vec2& scale, const Vec4& color) -> void;
        auto SubmitQuad(const Vec3& pos, const Vec2& scale, f32 rotation, const Vec4& color) -> void;
        auto SubmitQuad(const Vec3& pos, const Vec2& scale, f32 rotation, const Vec4& color, const Rc<Texture2D>& texture) -> void;
        auto EndFrame() const -> void;
        auto SetViewportSize(const Vec<2, i32>& size) -> void;

    private:
        auto StartBatch() -> void;
        auto FlushBatch() const -> void;
        auto IsBatchFull() const -> bool;

    private:
        static constexpr int MAX_TEXTURES = 32;

        u32 _max_quads = 0;
        u32 _quad_index_count = 0;
        Rc<Shader> _default_shader = {};
        Rc<VertexArray> _vertex_array = {};
        Rc<VertexBuffer> _vertex_buffer = {};
        Rc<Texture2D> _default_texture = {};
        u32 _texture_index = 1;
        Array<Rc<Texture2D>, MAX_TEXTURES> _texture_slots = {};
        QuadVertexData* _vertex_buffer_base = nullptr;
        QuadVertexData* _vertex_buffer_ptr = nullptr;
        Vec<2, u32> _viewport_size = {};
    };
} // namespace Cobalt

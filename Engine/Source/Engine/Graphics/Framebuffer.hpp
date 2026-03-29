// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Math.hpp"

namespace Cobalt::Engine
{
    enum class FramebufferAttachmentType : u8
    {
        None = 0,
        RGBA8,
        RedInteger,
    };

    class Framebuffer final
    {
    public:
        Framebuffer() = default;

        auto bind() -> void;
        auto unbind() const -> void;

        auto create(Vector<FramebufferAttachmentType> types, const Vec<2, u32> size, const u32 samples) -> void;
        auto resize(u32 width, u32 height) -> void;
        auto get_size() const -> Vec<2, u32>;
        auto clear_attachment(u32 index, i32 value) const -> void;
        auto get_color_attachment_id(u32 index) const -> i32;
        auto get_integer_at(u32 index, u32 x, u32 y) const -> i32;

    private:
        auto _create() -> void;
        auto _clear() -> void;
        auto _reallocate_textures() const -> void;

    private:
        Vector<FramebufferAttachmentType> m_attachment_types = {};
        Vector<u32> m_attachment_ids = {};
        Vec<2, u32> m_size = {};
        u32 m_renderer_id = 0;
        u32 m_samples = 0;
        bool m_is_resized = false;
    };
} // namespace Cobalt::Engine


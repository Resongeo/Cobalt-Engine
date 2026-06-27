// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Math.hpp"

namespace Cobalt
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

        auto Bind() -> void;
        auto Unbind() const -> void;

        auto Create(const Vector<FramebufferAttachmentType>& types, const Vec<2, u32> size, const u32 samples) -> void;
        auto Resize(u32 width, u32 height) -> void;
        auto ClearAttachment(u32 index, i32 value) const -> void;
        auto GetSize() const -> Vec<2, u32>;
        auto GetColorAttachmentID(u32 index) const -> i32;
        auto GetIntegerAt(u32 index, u32 x, u32 y) const -> i32;

    private:
        auto ReCreate() -> void;
        auto Clear() -> void;
        auto ReallocateTextures() const -> void;

    private:
        Vector<FramebufferAttachmentType> _attachment_types = {};
        Vector<u32> _attachment_ids = {};
        Vec<2, u32> _size = {};
        u32 _renderer_id = 0;
        u32 _samples = 0;
        bool _is_resized = false;
    };
} // namespace Cobalt

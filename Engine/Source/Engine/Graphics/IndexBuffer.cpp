// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/IndexBuffer.hpp"
#include "Engine/Core/Log.hpp"

namespace Cobalt
{
    IndexBuffer::~IndexBuffer() {
        CORE_INFO("Graphics::IndexBuffer: Deleting. ID: {}", _renderer_id);

        _renderer_id = 0;
    }

    auto IndexBuffer::Bind() const -> void {
    }

    auto IndexBuffer::Unbind() const -> void {
    }

    auto IndexBuffer::Create(const u32 count, const u32* indices) -> void {
        CORE_INFO("Graphics::IndexBuffer: Creating. ID: {}", _renderer_id);

        _count = count;
    }

    auto IndexBuffer::GetCount() const -> u32 {
        return _count;
    }
} // namespace Cobalt

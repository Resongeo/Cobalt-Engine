// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Core/Log.hpp"

#include <glad/gl.h>

namespace Cobalt
{
    namespace Utils
    {
        enum class AttachmentType : u8
        {
            Color = 0,
            Integer,
        };

        static auto attach_texture(const i32 index, const u32 id, const GLenum internal_format, const GLenum format,
                                   const u32 samples, const Vec2& size, const AttachmentType type) -> void {

            const bool is_multisampled = samples > 1;
            const auto texture_target = is_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

            if (is_multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, static_cast<i32>(size.x),
                                        static_cast<i32>(size.y), GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, internal_format, static_cast<i32>(size.x), static_cast<i32>(size.y), 0,
                             format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            switch (type) {
                case AttachmentType::Color:
                case AttachmentType::Integer:
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target, id, 0);
                    break;
            }
        }
    } // namespace Utils

    auto Framebuffer::Bind() -> void {
        if (_renderer_id == 0) {
            ReCreate();
        }

        if (_is_resized) {
            ReallocateTextures();
            _is_resized = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, _renderer_id);
        glViewport(0, 0, static_cast<i32>(_size.x), static_cast<i32>(_size.y));
    }

    auto Framebuffer::Unbind() const -> void {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    auto Framebuffer::Create(const Vector<FramebufferAttachmentType>& types, const Vec<2, u32> size, const u32 samples)
            -> void {
        _attachment_types = types;
        _size = size;
        _samples = samples;

        _attachment_ids.resize(_attachment_types.size());
        ReCreate();

        CORE_INFO(
            "Graphics::Framebuffer: Created. ID: {}. Attachment count: {}. Size {}x{}",
            _renderer_id, _attachment_types.size(), _size.x, _size.y
        );
    }

    auto Framebuffer::Resize(const u32 width, const u32 height) -> void {
        if (_size.x == width && _size.y == height) {
            return;
        }

        _size.x = width;
        _size.y = height;
        _is_resized = true;
    }

    auto Framebuffer::ClearAttachment(const u32 index, const i32 value) const -> void {
        glClearTexImage(_attachment_ids[index], 0, GL_RED_INTEGER, GL_INT, &value);
    }

    auto Framebuffer::GetSize() const -> Vec<2, u32> {
        return _size;
    }

    auto Framebuffer::GetColorAttachmentID(const u32 index) const -> i32 {
        if (index <= _attachment_ids.size()) {
            return _attachment_ids[index];
        }

        return -1;
    }

    auto Framebuffer::GetIntegerAt(u32 index, u32 x, u32 y) const -> i32 {
        if (_renderer_id == 0) {
            return -1;
        }

        i32 pixel_data;
        glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);

        return pixel_data;
    }

    auto Framebuffer::ReCreate() -> void {
        if (_renderer_id != 0) {
            Clear();
        }

        glGenFramebuffers(1, &_renderer_id);
        glBindFramebuffer(GL_FRAMEBUFFER, _renderer_id);

        const auto is_multisampled = _samples > 1;
        const auto texture_target = is_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        glCreateTextures(texture_target, _attachment_ids.size(), _attachment_ids.data());

        for (u32 i = 0; i < _attachment_ids.size(); i++) {
            glBindTexture(texture_target, _attachment_ids[i]);

            switch (_attachment_types[i]) {
                case FramebufferAttachmentType::RGBA8:
                    Utils::attach_texture(i, _attachment_ids[i], GL_RGBA8, GL_RGBA, _samples, _size, Utils::AttachmentType::Color);
                    break;
                case FramebufferAttachmentType::RedInteger:
                    Utils::attach_texture(i, _attachment_ids[i], GL_R32I, GL_RED_INTEGER, _samples, _size, Utils::AttachmentType::Integer);
                    break;
                case FramebufferAttachmentType::None: break;
            }
        }

        if (!_attachment_ids.empty()) {
            constexpr GLenum buffers[4] = {
                    GL_COLOR_ATTACHMENT0,
                    GL_COLOR_ATTACHMENT1,
                    GL_COLOR_ATTACHMENT2,
                    GL_COLOR_ATTACHMENT3,
            };
            glDrawBuffers(_attachment_ids.size(), buffers);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            CORE_ERROR("Graphics::Framebuffer: Framebuffer is not complete!");
        }
    }

    auto Framebuffer::Clear() -> void {
        glDeleteFramebuffers(1, &_renderer_id);
        glDeleteTextures(_attachment_ids.size(), _attachment_ids.data());
        _renderer_id = 0;
    }

    auto Framebuffer::ReallocateTextures() const -> void {
        const auto texture_target = _samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        for (u32 i = 0; i < _attachment_ids.size(); i++) {
            glBindTexture(texture_target, _attachment_ids[i]);

            // TODO: Maybe refactor this
            switch (_attachment_types[i]) {
                case FramebufferAttachmentType::RGBA8:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<i32>(_size.x), static_cast<i32>(_size.y), 0, GL_RGBA,
                                 GL_UNSIGNED_BYTE, nullptr);
                    break;
                case FramebufferAttachmentType::RedInteger:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, static_cast<i32>(_size.x), static_cast<i32>(_size.y), 0, GL_RED_INTEGER, GL_INT,
                                 nullptr);
                    break;
                case FramebufferAttachmentType::None: break;
            }
        }
    }
} // namespace Cobalt

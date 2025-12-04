// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/gl.h>

namespace Cobalt::Engine
{
    namespace Utils
    {
        enum class AttachmentType : u8
        {
            Color = 0,
            Integer,
        };
        
        static auto attach_texture(
            const i32 index,
            const u32 id,
            const GLenum internal_format,
            const GLenum format,
            const u32 samples,
            const Vec2& size,
            const AttachmentType type
        ) -> void {
            
            const bool is_multisampled = samples > 1;
            const auto texture_target = is_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

            if (is_multisampled)
            {
                glTexImage2DMultisample(
                    GL_TEXTURE_2D_MULTISAMPLE,
                    samples,
                    internal_format,
                    static_cast<i32>(size.x),
                    static_cast<i32>(size.y),
                    GL_FALSE
                );
            }
            else
            {
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    internal_format,
                    static_cast<i32>(size.x),
                    static_cast<i32>(size.y),
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    nullptr
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            switch (type)
            {
                case AttachmentType::Color:
                case AttachmentType::Integer:
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target, id, 0);
                    break;
            }
        }
    }
    
    auto Framebuffer::bind() -> void {
        if (m_renderer_id == 0) {
            _create();
        }

        if (m_is_resized) {
            _reallocate_textures();
            m_is_resized = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
        glViewport(0, 0, static_cast<i32>(m_size.x), static_cast<i32>(m_size.y));
    }

    auto Framebuffer::unbind() const -> void {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    auto Framebuffer::create(Vector<FramebufferAttachmentType> types, const Vec<2, u32> size, const u32 samples) -> void {
        m_attachment_types = types;
        m_size = size;
        m_samples = samples;

        m_attachment_ids.resize(m_attachment_types.size());
        _create();
        
        Logger::trace("framebuffer", "Created ID: {} Attachment count: {} Size {}x{}",
            m_renderer_id, m_attachment_types.size(), m_size.x, m_size.y);
    }

    auto Framebuffer::resize(const u32 width, const u32 height) -> void {
        if (m_size.x == width && m_size.y == height) {
            return;
        }

        m_size.x = width;
        m_size.y = height;
        m_is_resized = true;
    }

    auto Framebuffer::get_size() const -> Vec<2, u32> {
        return m_size;
    }

    auto Framebuffer::clear_attachment(const u32 index, const i32 value) const -> void {
        glClearTexImage(m_attachment_ids[index], 0, GL_RED_INTEGER, GL_INT, &value);
    }

    auto Framebuffer::get_color_attachment_id(const u32 index) const -> i32 {
        if (index <= m_attachment_ids.size()) {
            return m_attachment_ids[index];
        }

        return -1;
    }

    auto Framebuffer::get_integer_at(u32 index, u32 x, u32 y) const -> i32 {
        if (m_renderer_id == 0) {
            return -1;
        }

        i32 pixel_data;
        glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);

        return pixel_data;
    }

    auto Framebuffer::_create() -> void {
        if (m_renderer_id != 0) {
            _clear();
        }

        glGenFramebuffers(1, &m_renderer_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

        const auto is_multisampled = m_samples > 1;
        const auto texture_target = is_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        glCreateTextures(texture_target, m_attachment_ids.size(), m_attachment_ids.data());

        for (u32 i = 0; i < m_attachment_ids.size(); i++) {
            glBindTexture(texture_target, m_attachment_ids[i]);

            switch (m_attachment_types[i]) {
                case FramebufferAttachmentType::RGBA8:
                    Utils::attach_texture(i, m_attachment_ids[i], GL_RGBA8, GL_RGBA, m_samples, m_size, Utils::AttachmentType::Color);
                    break;
                case FramebufferAttachmentType::RedInteger:
                    Utils::attach_texture(i, m_attachment_ids[i], GL_R32I, GL_RED_INTEGER, m_samples, m_size, Utils::AttachmentType::Integer);
                    break;
                case FramebufferAttachmentType::None:
                    break;
            }
        }

        if (!m_attachment_ids.empty()) {
            constexpr GLenum buffers[4] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3,
            };
            glDrawBuffers(m_attachment_ids.size(), buffers);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::error("framebuffer", "Framebuffer is not complete!");
        }
    }

    auto Framebuffer::_clear() -> void {
        glDeleteFramebuffers(1, &m_renderer_id);
        glDeleteTextures(m_attachment_ids.size(), m_attachment_ids.data());
        m_renderer_id = 0;
    }

    auto Framebuffer::_reallocate_textures() const -> void {
        const auto texture_target = m_samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        
        for (u32 i = 0; i < m_attachment_ids.size(); i++) {
            glBindTexture(texture_target, m_attachment_ids[i]);

            // TODO: Maybe refactor this
            switch (m_attachment_types[i])
            {
                case FramebufferAttachmentType::RGBA8:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<i32>(m_size.x), static_cast<i32>(m_size.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                    break;
                case FramebufferAttachmentType::RedInteger:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, static_cast<i32>(m_size.x), static_cast<i32>(m_size.y), 0, GL_RED_INTEGER, GL_INT, nullptr);
                    break;
                case FramebufferAttachmentType::None:
                    break;
            }
        }
    }
}

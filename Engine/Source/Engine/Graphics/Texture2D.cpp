// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Core/Log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>
#include <stb_image.h>

namespace Cobalt
{
    auto Texture2D::load_from_file(const Filepath& path) -> bool {
        i32 width, height, channels;

        const auto path_str = path.string();
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path_str.c_str(), &width, &height, &channels, 0);

        m_width = width;
        m_height = height;

        CORE_INFO("Graphics::Texture2D: Loading from file: {} Size: {}x{}", path_str, m_width, m_height);

        const auto internal_format = channels == 4 ? GL_RGBA8 : GL_RGB8;
        const auto data_format = channels == 4 ? GL_RGBA : GL_RGB;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
        glTextureStorage2D(m_renderer_id, 1, internal_format, m_width, m_height);

        glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, data_format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        return true;
    }

    auto Texture2D::create_with_size(const u32 width, const u32 height) -> bool {
        constexpr auto internal_format = GL_RGBA8;
        constexpr auto data_format = GL_RGBA;

        m_width = width;
        m_height = height;

        CORE_INFO("Graphics::Texture2D: Creating. Size: {}x{}", m_width, m_height);

        auto temp_buffer = Vector<unsigned char>{};
        temp_buffer.resize(m_width * m_height * 4, 255);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
        glTextureStorage2D(m_renderer_id, 1, internal_format, m_width, m_height);

        glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, data_format, GL_UNSIGNED_BYTE,
                            temp_buffer.data());

        return true;
    }

    auto Texture2D::bind_slot(const u32 slot) const -> void {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_renderer_id);
    }

    auto Texture2D::get_width() const -> u32 {
        return m_width;
    }

    auto Texture2D::get_height() const -> u32 {
        return m_height;
    }

    auto Texture2D::get_renderer_id() const -> u32 {
        return m_renderer_id;
    }

    Texture2D::~Texture2D() {
        CORE_INFO("Graphics::Texture2D Deleting. ID: {}", m_renderer_id);

        glDeleteTextures(1, &m_renderer_id);
        m_renderer_id = 0;
    }
} // namespace Cobalt

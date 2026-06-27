// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Core/Log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>
#include <stb_image.h>

namespace Cobalt
{
    auto Texture2D::LoadFromFile(const Filepath& path) -> bool {
        i32 tex_width, tex_height, channels;

        const auto path_str = path.string();
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path_str.c_str(), &tex_width, &tex_height, &channels, 0);

        _width = tex_width;
        _height = tex_height;

        CORE_INFO("Graphics::Texture2D: Loading from file: {} Size: {}x{}", path_str, _width, _height);

        const auto internal_format = channels == 4 ? GL_RGBA8 : GL_RGB8;
        const auto data_format = channels == 4 ? GL_RGBA : GL_RGB;

        glCreateTextures(GL_TEXTURE_2D, 1, &_renderer_id);
        glTextureStorage2D(_renderer_id, 1, internal_format, _width, _height);

        glTextureParameteri(_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureSubImage2D(_renderer_id, 0, 0, 0, _width, _height, data_format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        return true;
    }

    auto Texture2D::CreateWithSize(const u32 width, const u32 height) -> bool {
        constexpr auto internal_format = GL_RGBA8;
        constexpr auto data_format = GL_RGBA;

        _width = width;
        _height = height;

        CORE_INFO("Graphics::Texture2D: Creating. Size: {}x{}", _width, _height);

        auto temp_buffer = Vector<unsigned char>{};
        temp_buffer.resize(_width * _height * 4, 255);

        glCreateTextures(GL_TEXTURE_2D, 1, &_renderer_id);
        glTextureStorage2D(_renderer_id, 1, internal_format, _width, _height);

        glTextureParameteri(_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureSubImage2D(_renderer_id, 0, 0, 0, _width, _height, data_format, GL_UNSIGNED_BYTE,
                            temp_buffer.data());

        return true;
    }

    auto Texture2D::BindSlot(const u32 slot) const -> void {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, _renderer_id);
    }

    auto Texture2D::GetWidth() const -> u32 {
        return _width;
    }

    auto Texture2D::GetHeight() const -> u32 {
        return _height;
    }

    auto Texture2D::GetRendererID() const -> u32 {
        return _renderer_id;
    }

    Texture2D::~Texture2D() {
        CORE_INFO("Graphics::Texture2D Deleting. ID: {}", _renderer_id);

        glDeleteTextures(1, &_renderer_id);
        _renderer_id = 0;
    }
} // namespace Cobalt

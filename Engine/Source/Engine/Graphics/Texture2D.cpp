// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Core/Log.hpp"

#include <glad/gl.h>
#include <SDL3_image/SDL_image.h>

namespace Cobalt
{
    auto Texture2D::LoadFromFile(const Filepath& path) -> bool {
        const auto path_str = path.string();
        const auto surface = IMG_Load(path_str.c_str());
        SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);

        _width = surface->w;
        _height = surface->h;

        CORE_INFO("Graphics::Texture2D: Loading from file: {} Size: {}x{}", path_str, _width, _height);

        GLenum internal_format = {};
        GLenum data_format = {};

        if (surface->format == SDL_PIXELFORMAT_RGBA32) {
            internal_format = GL_RGBA8;
            data_format = GL_RGBA;
        } else if (surface->format == SDL_PIXELFORMAT_RGB24) {
            internal_format = GL_RGB8;
            data_format = GL_RGB;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &_renderer_id);
        glTextureStorage2D(_renderer_id, 1, internal_format, _width, _height);

        glTextureParameteri(_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureSubImage2D(_renderer_id, 0, 0, 0, _width, _height, data_format, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_DestroySurface(surface);

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

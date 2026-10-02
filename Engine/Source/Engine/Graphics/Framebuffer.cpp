// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Framebuffer.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Rendering/RHI.hpp"

namespace Cobalt
{
    Framebuffer::Framebuffer(u32 width, u32 height, SDL_GPUTextureFormat color_format) :
        _width(width), _height(height), _color_format(color_format) {
        Invalidate();
    }

    Framebuffer::~Framebuffer() {
        Release();
    }

    void Framebuffer::Resize(u32 width, u32 height) {
        if (width == 0 || height == 0 || (_width == width && _height == height)) {
            return;
        }

        _width = width;
        _height = height;

        Release();
        Invalidate();
    }

    void Framebuffer::Release() {
        SDL_GPUDevice* device = RHI::GetDevice();

        if (_color_texture) {
            SDL_ReleaseGPUTexture(device, _color_texture);
            _color_texture = nullptr;
        }
    }

    void Framebuffer::Invalidate() {
        SDL_GPUDevice* device = RHI::GetDevice();

        // 1. Create Color Attachment
        SDL_GPUTextureCreateInfo color_info = {};
        color_info.type = SDL_GPU_TEXTURETYPE_2D;
        color_info.format = _color_format;
        color_info.width = _width;
        color_info.height = _height;
        //color_info.layer_count_or_depth = 1;
        color_info.num_levels = 1;
        color_info.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;

        _color_texture = SDL_CreateGPUTexture(device, &color_info);
        if (!_color_texture) {
            CORE_ERROR("Framebuffer: Failed to create color texture!");
        }
    }
} // namespace Cobalt

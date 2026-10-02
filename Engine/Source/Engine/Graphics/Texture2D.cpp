// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Texture2D.hpp"
#include "Engine/Rendering/RHI.hpp"
#include "Engine/Core/Log.hpp"

#include <SDL3_image/SDL_image.h>

namespace Cobalt
{
    auto Texture2D::LoadFromFile(const Filepath& path) -> bool {
        SDL_Surface* surface = IMG_Load(path.string().c_str());
        if (!surface) {
            CORE_ERROR("Failed to load image from path: {}. Error: {}", path.string(), SDL_GetError());
            return false;
        }

        SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);
        SDL_Surface* formatted_surface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
        SDL_DestroySurface(surface);

        if (!formatted_surface) {
            CORE_ERROR("Failed to convert surface to RGBA32 for path: {}", path.string());
            return false;
        }

        _width = static_cast<u32>(formatted_surface->w);
        _height = static_cast<u32>(formatted_surface->h);

        auto* device = RHI::GetDevice();

        SDL_GPUTextureCreateInfo texture_info = {};
        texture_info.type = SDL_GPU_TEXTURETYPE_2D;
        texture_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        texture_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
        texture_info.width = _width;
        texture_info.height = _height;
        texture_info.layer_count_or_depth = 1;
        texture_info.num_levels = 1;

        _texture = SDL_CreateGPUTexture(device, &texture_info);
        if (!_texture) {
            CORE_ERROR("Failed to create GPU texture for path: {}", path.string());
            SDL_DestroySurface(formatted_surface);
            return false;
        }

        u32 image_size = _width * _height * 4;
        SDL_GPUTransferBufferCreateInfo transfer_info = {};
        transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transfer_info.size = image_size;

        SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transfer_info);
        if (!transfer_buffer) {
            CORE_ERROR("Failed to create transfer buffer for texture upload.");
            SDL_DestroySurface(formatted_surface);
            return false;
        }

        void* map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
        SDL_memcpy(map, formatted_surface->pixels, image_size);
        SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

        SDL_GPUCommandBuffer* cmd_buf = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd_buf);

        SDL_GPUTextureTransferInfo transfer_info_src = {};
        transfer_info_src.transfer_buffer = transfer_buffer;
        transfer_info_src.offset = 0;
        transfer_info_src.pixels_per_row = _width;
        transfer_info_src.rows_per_layer = _height;

        SDL_GPUTextureRegion dst_region = {};
        dst_region.texture = _texture;
        dst_region.w = _width;
        dst_region.h = _height;
        dst_region.d = 1;

        SDL_UploadToGPUTexture(copy_pass, &transfer_info_src, &dst_region, false);
        SDL_EndGPUCopyPass(copy_pass);

        SDL_SubmitGPUCommandBuffer(cmd_buf);
        SDL_WaitForGPUIdle(device);

        SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
        SDL_DestroySurface(formatted_surface);

        CORE_INFO("Loaded Texture: {}", path.string());

        return true;
    }

    auto Texture2D::CreateWithSize(const u32 width, const u32 height) -> bool {
        _width = width;
        _height = height;

        auto* device = RHI::GetDevice();
        SDL_GPUTextureCreateInfo texture_info = {};
        texture_info.type = SDL_GPU_TEXTURETYPE_2D;
        texture_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        texture_info.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
        texture_info.width = _width;
        texture_info.height = _height;
        texture_info.layer_count_or_depth = 1;
        texture_info.num_levels = 1;

        _texture = SDL_CreateGPUTexture(device, &texture_info);
        return _texture != nullptr;
    }

    auto Texture2D::GetWidth() const -> u32 {
        return _width;
    }

    auto Texture2D::GetHeight() const -> u32 {
        return _height;
    }

    auto Texture2D::GetGPUTexture() const -> SDL_GPUTexture* {
        return _texture;
    }

    auto Texture2D::Destroy() -> void {
        if (!_texture) return;

        CORE_INFO("Graphics::Texture2D Deleting GPU Texture.");
        RHI::DestroyTexture(_texture);
    }

    Texture2D::~Texture2D() {
        Destroy();
    }
} // namespace Cobalt
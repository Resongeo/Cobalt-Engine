// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Error.hpp"
#include "Engine/Core/Types/Base.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>
#include <SDL3_shadercross/SDL_shadercross.h>

namespace Cobalt
{
    enum class RHIError
    {
        CreateGPUDevice,
        ClaimWindowForGPUDevice,
        SetGPUSwapchainParameters,
        ShaderCrossInit,
        SwapchainTextureFormatInvalid,
    };

    namespace RHI
    {
        auto Initialize() -> Result<void, RHIError>;
        auto Shutdown() -> void;

        auto GetDevice() -> SDL_GPUDevice*;
        auto GetWindow() -> SDL_Window*;
        auto GetPresentMode() -> SDL_GPUPresentMode;
        auto GetSwapchainTextureFormat() -> SDL_GPUTextureFormat;

        auto CreateBuffer(u32 size, SDL_GPUBufferUsageFlags usage) -> SDL_GPUBuffer*;
        auto CreateSampler(SDL_GPUFilter filter = SDL_GPU_FILTER_NEAREST) -> SDL_GPUSampler*;
        auto CreateTexture(u32 width, u32 height, SDL_GPUTextureFormat format, SDL_GPUTextureUsageFlags usage) -> SDL_GPUTexture*;
        auto CreateShader(SDL_ShaderCross_ShaderStage stage, const char* source, const char* entrypoint, u32 samplers, u32 storage_buffers, u32 storage_textures, u32 uniform_buffers) -> SDL_GPUShader*;
        auto CreateTransferBuffer(u32 size, SDL_GPUTransferBufferUsage usage) -> SDL_GPUTransferBuffer*;

        auto CreateQuadPipeline(SDL_GPUShader* vertex, SDL_GPUShader* fragment, SDL_GPUTextureFormat color_target_format)
                -> SDL_GPUGraphicsPipeline*;

        auto DestroyTexture(SDL_GPUTexture* texture) -> void;
    } // namespace RHI
} // namespace Cobalt

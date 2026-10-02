// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Rendering/RHI.hpp"
#include "Engine/Core/Defines.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Platform/Window.hpp"

namespace Cobalt
{
    struct RHIState
    {
        SDL_GPUDevice* device = nullptr;
        SDL_Window* window = nullptr;
        SDL_GPUPresentMode present_mode = SDL_GPU_PRESENTMODE_VSYNC;
        SDL_GPUTextureFormat swapchain_texture_format = SDL_GPU_TEXTUREFORMAT_INVALID;
    };

    static RHIState state;

    auto RHI::Initialize() -> Result<void, RHIError> {
        state.window = Window::GetHandle();

#if defined(CONFIGURATION_DEBUG)
        constexpr bool debug_mode = true;
#else
        constexpr bool debug_mode = false;
#endif

        state.device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, debug_mode, nullptr);
        if (!state.device) {
            CORE_ERROR("{}", SDL_GetError());
            return Err(RHIError::CreateGPUDevice);
        }

        if (!SDL_ClaimWindowForGPUDevice(state.device, state.window)) {
            CORE_ERROR("{}", SDL_GetError());
            return Err(RHIError::ClaimWindowForGPUDevice);
        }

        if (SDL_WindowSupportsGPUPresentMode(state.device, state.window, SDL_GPU_PRESENTMODE_MAILBOX)) {
            state.present_mode = SDL_GPU_PRESENTMODE_MAILBOX;
        }

        if (!SDL_SetGPUSwapchainParameters(state.device, state.window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, state.present_mode)) {
            CORE_ERROR("{}", SDL_GetError());
            return Err(RHIError::SetGPUSwapchainParameters);
        }

        if (!SDL_ShaderCross_Init()) {
            CORE_ERROR("{}", SDL_GetError());
            return Err(RHIError::ShaderCrossInit);
        }

        state.swapchain_texture_format = SDL_GetGPUSwapchainTextureFormat(state.device, state.window);;
        if (state.swapchain_texture_format == SDL_GPU_TEXTUREFORMAT_INVALID) {
            CORE_ERROR("{}", SDL_GetError());
            return Err(RHIError::SwapchainTextureFormatInvalid);
        }

        return {};
    }

    auto RHI::Shutdown() -> void {
        SDL_WaitForGPUIdle(state.device);
        SDL_ShaderCross_Quit();

        if (state.device) {
            SDL_ReleaseWindowFromGPUDevice(state.device, state.window);
            SDL_DestroyGPUDevice(state.device);
            state.device = nullptr;
        }
    }

    auto RHI::GetDevice() -> SDL_GPUDevice* {
        return state.device;
    }

    auto RHI::GetWindow() -> SDL_Window* {
        return state.window;
    }

    auto RHI::GetPresentMode() -> SDL_GPUPresentMode {
        return state.present_mode;
    }

    auto RHI::GetSwapchainTextureFormat() -> SDL_GPUTextureFormat {
        return state.swapchain_texture_format;
    }

    auto RHI::CreateBuffer(const u32 size, const SDL_GPUBufferUsageFlags usage) -> SDL_GPUBuffer* {
        SDL_GPUBufferCreateInfo create_info = {};
        create_info.size = size;
        create_info.usage = usage;

        auto* buffer = SDL_CreateGPUBuffer(state.device, &create_info);
        if (!buffer) {
            CORE_ERROR("{}", SDL_GetError());
            return nullptr;
        }

        return buffer;
    }

    auto RHI::CreateSampler(const SDL_GPUFilter filter) -> SDL_GPUSampler* {
        SDL_GPUSamplerCreateInfo create_info = {};
        create_info.min_filter = filter;
        create_info.mag_filter = filter;
        create_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
        create_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        create_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        create_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        auto* sampler = SDL_CreateGPUSampler(state.device, &create_info);
        if (!sampler) {
            CORE_ERROR("{}", SDL_GetError());
            return nullptr;
        }

        return sampler;
    }

    auto RHI::CreateTexture(const u32 width, const u32 height, const SDL_GPUTextureFormat format, const SDL_GPUTextureUsageFlags usage)
            -> SDL_GPUTexture* {
        SDL_GPUTextureCreateInfo create_info = {};
        create_info.type = SDL_GPU_TEXTURETYPE_2D;
        create_info.format = format;
        create_info.width = width;
        create_info.height = height;
        create_info.layer_count_or_depth = 1;
        create_info.num_levels = 1;
        create_info.usage = usage;

        auto* texture = SDL_CreateGPUTexture(state.device, &create_info);
        if (!texture) {
            CORE_ERROR("{}", SDL_GetError());
            return nullptr;
        }

        return texture;
    }

    auto RHI::CreateShader(SDL_ShaderCross_ShaderStage stage, const char* source, const char* entrypoint, const u32 samplers,
                           const u32 storage_buffers, const u32 storage_textures, const u32 uniform_buffers) -> SDL_GPUShader* {
        SDL_ShaderCross_HLSL_Info hlsl_info = {};
        hlsl_info.source = source;
        hlsl_info.entrypoint = entrypoint;
        hlsl_info.shader_stage = stage;
        hlsl_info.props = 0;

        usize spirv_size = 0;
        auto* spirv_bytecode = SDL_ShaderCross_CompileSPIRVFromHLSL(&hlsl_info, &spirv_size);
        if (!spirv_bytecode) {
            SDL_Log("SDL_ShaderCross_CompileSPIRVFromHLSL failed: %s", SDL_GetError());
            return nullptr;
        }

        SDL_GPUShaderCreateInfo create_info = {};
        create_info.code_size = spirv_size;
        create_info.code = static_cast<u8*>(spirv_bytecode);
        create_info.entrypoint = entrypoint;
        create_info.format = SDL_GPU_SHADERFORMAT_SPIRV;
        create_info.stage = static_cast<SDL_GPUShaderStage>(stage);
        create_info.num_samplers = samplers;
        create_info.num_storage_textures = storage_textures;
        create_info.num_storage_buffers = storage_buffers;
        create_info.num_uniform_buffers = uniform_buffers;

        auto* shader = SDL_CreateGPUShader(state.device, &create_info);
        if (!shader) {
            CORE_ERROR("{}", SDL_GetError());
            SDL_free(spirv_bytecode);

            return nullptr;
        }

        SDL_free(spirv_bytecode);

        return shader;
    }

    auto RHI::CreateTransferBuffer(const u32 size, const SDL_GPUTransferBufferUsage usage) -> SDL_GPUTransferBuffer* {
        SDL_GPUTransferBufferCreateInfo create_info = {};
        create_info.size = size;
        create_info.usage = usage;

        auto* transfer_buffer = SDL_CreateGPUTransferBuffer(state.device, &create_info);
        if (!transfer_buffer) {
            CORE_ERROR("{}", SDL_GetError());
            return nullptr;
        }

        return transfer_buffer;
    }

    auto RHI::CreateQuadPipeline(SDL_GPUShader* vertex, SDL_GPUShader* fragment, const SDL_GPUTextureFormat color_target_format)
            -> SDL_GPUGraphicsPipeline* {
        SDL_GPUGraphicsPipelineCreateInfo create_info = {};
        create_info.vertex_shader = vertex;
        create_info.fragment_shader = fragment;
        create_info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

        SDL_GPUVertexBufferDescription vbo_desc = {};
        vbo_desc.slot = 0;
        vbo_desc.pitch = sizeof(float) * 9; // pos(2)+uv(2)+color(4)+tex_id(1)

        SDL_GPUVertexAttribute attributes[4] = {};
        attributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, 0}; // Position
        attributes[1] = {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, sizeof(float) * 2}; // UV
        attributes[2] = {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, sizeof(float) * 4}; // Color
        attributes[3] = {3, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT, sizeof(float) * 8}; // TexIndex

        create_info.vertex_input_state.vertex_buffer_descriptions = &vbo_desc;
        create_info.vertex_input_state.num_vertex_buffers = 1;
        create_info.vertex_input_state.vertex_attributes = attributes;
        create_info.vertex_input_state.num_vertex_attributes = 4;

        SDL_GPUColorTargetDescription color_desc = {};
        color_desc.format = color_target_format;
        color_desc.blend_state.enable_blend = true;
        color_desc.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        color_desc.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        color_desc.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
        color_desc.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
        color_desc.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        color_desc.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;

        create_info.target_info.color_target_descriptions = &color_desc;
        create_info.target_info.num_color_targets = 1;

        auto* graphics_pipeline = SDL_CreateGPUGraphicsPipeline(state.device, &create_info);
        if (!graphics_pipeline) {
            CORE_ERROR("{}", SDL_GetError());
            return nullptr;
        }

        return graphics_pipeline;
    }

    auto RHI::DestroyTexture(SDL_GPUTexture* texture) -> void {
        SDL_ReleaseGPUTexture(state.device, texture);
        texture = nullptr;
    }
} // namespace Cobalt

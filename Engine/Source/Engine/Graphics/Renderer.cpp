// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/File.hpp"
#include "Engine/Core/Project.hpp"
#include "Engine/Core/Types/Math.hpp"

namespace Cobalt
{
    constexpr Uint32 MAX_SPRITES = 8192;

    struct RendererState
    {
        SDL_GPUGraphicsPipeline* pipeline = nullptr;
        SDL_GPUSampler* default_sampler = nullptr;
        SDL_GPUBuffer* storage_buffer = nullptr;
        SDL_GPUTransferBuffer* transfer_buffer = nullptr;

        SDL_GPUCommandBuffer* current_cmd_buf = nullptr;
        SDL_GPUTexture* swapchain_texture = nullptr;

        std::vector<RenderPass> pass_queue;
    };

    static RendererState state;

    auto Renderer::Initialize() -> Result<void, RendererError> {
        auto vert_path = Project::GetEditorAssetsPath() / "Shaders" / "DefaultQuad.vert.hlsl";
        auto frag_path = Project::GetEditorAssetsPath() / "Shaders" / "DefaultQuad.frag.hlsl";

        auto vert_src = File::Read(vert_path);
        auto frag_src = File::Read(frag_path);

        auto* device = RHI::GetDevice();
        auto* vertShader = RHI::CreateShader(SDL_SHADERCROSS_SHADERSTAGE_VERTEX, vert_src.c_str(), "main", 0, 1, 0, 1);
        auto* fragShader = RHI::CreateShader(SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT, frag_src.c_str(), "main", 1, 0, 0, 0);

        if (!vertShader || !fragShader) return Err(RendererError::DefaultShaderCreation);

        SDL_GPUVertexBufferDescription vbo_desc = {};
        vbo_desc.slot = 0;
        vbo_desc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        vbo_desc.pitch = sizeof(SpriteInstance);

        SDL_GPURasterizerState rasterizer_state = {};
        rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
        rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
        rasterizer_state.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

        SDL_GPUMultisampleState multisample_state = {};
        multisample_state.sample_count = SDL_GPU_SAMPLECOUNT_1;

        SDL_GPUDepthStencilState depth_stencil_state = {};
        depth_stencil_state.enable_depth_test = false;
        depth_stencil_state.enable_depth_write = false;

        SDL_GPUColorTargetBlendState blend_state = {};
        blend_state.enable_blend = true;
        blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
        blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
        blend_state.color_write_mask =
                SDL_GPU_COLORCOMPONENT_R | SDL_GPU_COLORCOMPONENT_G | SDL_GPU_COLORCOMPONENT_B | SDL_GPU_COLORCOMPONENT_A;

        SDL_GPUColorTargetDescription color_desc = {};
        color_desc.format = RHI::GetSwapchainTextureFormat();
        color_desc.blend_state = blend_state;

        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.vertex_shader = vertShader;
        pipelineInfo.fragment_shader = fragShader;
        pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
        pipelineInfo.rasterizer_state = rasterizer_state;
        pipelineInfo.multisample_state = multisample_state;
        pipelineInfo.depth_stencil_state = depth_stencil_state;
        pipelineInfo.target_info.num_color_targets = 1;
        pipelineInfo.target_info.color_target_descriptions = &color_desc;

        state.pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);
        if (!state.pipeline) return Err(RendererError::CreateGraphicsPipeline);

        SDL_ReleaseGPUShader(device, vertShader);
        SDL_ReleaseGPUShader(device, fragShader);

        state.default_sampler = RHI::CreateSampler(SDL_GPU_FILTER_NEAREST);
        state.storage_buffer = RHI::CreateBuffer(MAX_SPRITES * sizeof(SpriteInstance), SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ);
        state.transfer_buffer = RHI::CreateTransferBuffer(MAX_SPRITES * sizeof(SpriteInstance), SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD);

        state.pass_queue.reserve(16);

        return {};
    }

    auto Renderer::Shutdown() -> void {
        auto* device = RHI::GetDevice();
        SDL_ReleaseGPUGraphicsPipeline(device, state.pipeline);
        SDL_ReleaseGPUSampler(device, state.default_sampler);
        SDL_ReleaseGPUBuffer(device, state.storage_buffer);
        SDL_ReleaseGPUTransferBuffer(device, state.transfer_buffer);
    }

    auto Renderer::BeginFrame() -> void {
        state.current_cmd_buf = SDL_AcquireGPUCommandBuffer(RHI::GetDevice());
        SDL_WaitAndAcquireGPUSwapchainTexture(state.current_cmd_buf, RHI::GetWindow(), &state.swapchain_texture, nullptr, nullptr);
        state.pass_queue.clear();
    }

    auto Renderer::SubmitPass(const RenderPass& pass) -> void {
        state.pass_queue.push_back(pass);
    }

    auto Renderer::ExecutePasses() -> void {
        if (state.pass_queue.empty() || !state.current_cmd_buf) return;

        int window_w = 0, window_h = 0;
        SDL_GetWindowSizeInPixels(RHI::GetWindow(), &window_w, &window_h);

        for (const auto& pass : state.pass_queue) {
            if (pass.batches.empty()) continue;

            const u32 target_width = pass.width ? pass.width : static_cast<u32>(window_w);
            const u32 target_height = pass.height ? pass.height : static_cast<u32>(window_h);

            const auto projection = pass.camera->GetProjection({target_width, target_height});

            bool is_first_batch = true;
            for (const auto& batch : pass.batches) {
                if (batch.sprites.empty() || !batch.texture) continue;

                const auto count = static_cast<u32>(batch.sprites.size());
                const auto data_size = static_cast<u32>(count * sizeof(SpriteInstance));

                const auto mapped = SDL_MapGPUTransferBuffer(RHI::GetDevice(), state.transfer_buffer, true);
                SDL_memcpy(mapped, batch.sprites.data(), data_size);
                SDL_UnmapGPUTransferBuffer(RHI::GetDevice(), state.transfer_buffer);

                const auto copy_pass = SDL_BeginGPUCopyPass(state.current_cmd_buf);
                const auto src_loc = SDL_GPUTransferBufferLocation{state.transfer_buffer, 0};
                const auto dst_region = SDL_GPUBufferRegion{state.storage_buffer, 0, data_size};
                SDL_UploadToGPUBuffer(copy_pass, &src_loc, &dst_region, true);
                SDL_EndGPUCopyPass(copy_pass);

                SDL_GPUColorTargetInfo color_info = {};
                color_info.texture = pass.render_target ? pass.render_target : state.swapchain_texture;
                color_info.load_op = (is_first_batch && pass.clear) ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
                color_info.store_op = SDL_GPU_STOREOP_STORE;
                color_info.clear_color =
                        SDL_FColor{pass.camera->clear_color.r, pass.camera->clear_color.g, pass.camera->clear_color.b, 1.0};

                auto* render_pass = SDL_BeginGPURenderPass(state.current_cmd_buf, &color_info, 1, nullptr);
                SDL_BindGPUGraphicsPipeline(render_pass, state.pipeline);

                SDL_PushGPUVertexUniformData(state.current_cmd_buf, 0, &projection, sizeof(Mat4));

                SDL_BindGPUVertexStorageBuffers(render_pass, 0, &state.storage_buffer, 1);

                const auto binding = SDL_GPUTextureSamplerBinding{batch.texture, state.default_sampler};
                SDL_BindGPUFragmentSamplers(render_pass, 0, &binding, 1);

                SDL_DrawGPUPrimitives(render_pass, count * 6, 1, 0, 0);

                SDL_EndGPURenderPass(render_pass);

                is_first_batch = false;
            }
        }

        state.pass_queue.clear();
    }

    auto Renderer::EndFrame() -> void {
        ExecutePasses();
        SDL_SubmitGPUCommandBuffer(state.current_cmd_buf);
        state.current_cmd_buf = nullptr;
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Error.hpp"
#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Rendering/RHI.hpp"
#include "Engine/Graphics/Camera.hpp"

namespace Cobalt
{
    struct alignas(16) SpriteInstance
    {
        float x, y, z;
        float rotation;
        float w, h;
        float padding_a, padding_b;
        float tex_u, tex_v, tex_w, tex_h;
        float r, g, b, a;
    };

    struct SpriteBatch
    {
        SDL_GPUTexture* texture;
        std::vector<SpriteInstance> sprites;
    };

    struct RenderPass
    {
        // NOTE: If target is nullptr it will target the Swapchain
        SDL_GPUTexture* render_target = nullptr;
        u32 width = 0;
        u32 height = 0;
        const Camera* camera = {};
        bool clear = true;
        Vector<SpriteBatch> batches;
    };

    enum class RendererError {
        DefaultShaderCreation,
        CreateGraphicsPipeline,
    };

    namespace Renderer
    {
        auto Initialize() -> Result<void, RendererError>;
        auto Shutdown() -> void;

        auto BeginFrame() -> void;
        auto EndFrame() -> void;

        auto SubmitPass(const RenderPass& pass) -> void;
        auto ExecutePasses() -> void;
    }
}
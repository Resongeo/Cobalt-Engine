// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Math.hpp"

#include <SDL3/SDL.h>

namespace Cobalt
{
    class Framebuffer {
    public:
        Framebuffer() = default;
        Framebuffer(u32 width, u32 height, SDL_GPUTextureFormat color_format);
        ~Framebuffer();

        void Resize(u32 width, u32 height);
        void Release();

        SDL_GPUTexture* GetColorTexture() const { return _color_texture; }
        Vec2 GetSize() const { return { (float)_width, (float)_height }; }

    private:
        void Invalidate();

        u32 _width = 0, _height = 0;
        SDL_GPUTexture* _color_texture = nullptr;
        SDL_GPUTextureFormat _color_format;
    };
} // namespace Cobalt

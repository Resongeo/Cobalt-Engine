// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"

struct SDL_GPUTexture;

namespace Cobalt
{
    struct RenderTarget
    {
        SDL_GPUTexture* texture = nullptr;
        u32 width = 0;
        u32 height = 0;};
}
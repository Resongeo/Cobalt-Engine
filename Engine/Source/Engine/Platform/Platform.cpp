// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Platform/Platform.hpp"
#include "Engine/Core/Log.hpp"

#include <SDL3/SDL.h>

namespace Cobalt
{
    auto Platform::sdl3_init() -> bool {
        return true;
    }

    auto Platform::opengl_init() -> bool {

        return true;
    }

    auto Platform::destroy() -> void {
        SDL_Quit();
    }
} // namespace Cobalt

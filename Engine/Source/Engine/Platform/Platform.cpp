// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Platform/Platform.hpp"
#include "Engine/Core/Log.hpp"

#include <SDL3/SDL.h>

namespace Cobalt
{
    auto Platform::InitSDL3() -> bool {
        return true;
    }

    auto Platform::InitOpenGL() -> bool {
        return true;
    }

    auto Platform::ShutDown() -> void {
        SDL_Quit();
    }
} // namespace Cobalt

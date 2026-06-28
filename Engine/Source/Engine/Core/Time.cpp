// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Time.hpp"

#include <SDL3/SDL.h>

namespace Cobalt
{
    auto Time::Init() -> void {
        _current_time = SDL_GetTicks();
        _last_time = _current_time;
    }

    auto Time::Update() -> void {
        _current_time = SDL_GetTicks();
        _delta_time = static_cast<f32>(_current_time - _last_time) / 1000.0f;
        _last_time = _current_time;
    }

    auto Time::GetDeltaTime() -> f32 {
        return _delta_time;
    }
} // namespace Cobalt

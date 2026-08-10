// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Profiling/ScopedFrameProfileTimer.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"

namespace Cobalt
{
    ScopedFrameProfileTimer::ScopedFrameProfileTimer(const String& name) : _name(name) {
        _start_time_point = std::chrono::steady_clock::now();
    }

    ScopedFrameProfileTimer::~ScopedFrameProfileTimer() {
        const auto end_time_point = std::chrono::steady_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time_point - _start_time_point);

        FrameProfiler::Get().SubmitData(_name, duration.count());
    }
} // namespace Cobalt

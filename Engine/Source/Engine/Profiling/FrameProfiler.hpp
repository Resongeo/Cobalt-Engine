// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Profiling/ScopedFrameProfileTimer.hpp"
#include "Engine/Core/Types/Base.hpp"

namespace Cobalt
{
    struct FrameProfilerData
    {
        u64 frame_time = 0;
        HashMap<String, u64> event_times = {};
    };

    class FrameProfiler final
    {
    public:
        auto BeginFrame() -> void;
        auto EndFrame() -> void;
        auto SubmitData(const String& name, u64 time_micro_sec) -> void;
        auto GetLastFrameData() -> FrameProfilerData&;

        static auto Get() -> FrameProfiler&;

    private:
        FrameProfilerData _data = {};
    };

#define CONCAT_INNER(a, b) a##b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define FRAME_PROFILER_BEGIN() FrameProfiler::Get().BeginFrame()
#define FRAME_PROFILER_END() FrameProfiler::Get().EndFrame()
#define FRAME_PROFILER_EVENT(name) ScopedFrameProfileTimer CONCAT(_profile_timer_, __LINE__)(name)
}
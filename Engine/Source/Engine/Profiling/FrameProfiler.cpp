// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Profiling/FrameProfiler.hpp"

namespace Cobalt
{
    static std::chrono::steady_clock::time_point start_time_point;

    auto FrameProfiler::BeginFrame() -> void {
        _data.event_times.clear();
        start_time_point = std::chrono::steady_clock::now();
    }

    auto FrameProfiler::EndFrame() -> void {
        const auto end_time_point = std::chrono::steady_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time_point - start_time_point);

        _data.frame_time = duration.count();
    }

    auto FrameProfiler::SubmitData(const String& name, const u64 time_micro_sec) -> void {
        _data.event_times[name] = time_micro_sec;
    }

    auto FrameProfiler::GetLastFrameData() -> FrameProfilerData& {
        return _data;
    }

    auto FrameProfiler::Get() -> FrameProfiler& {
        static FrameProfiler instance;
        return instance;
    }
} // namespace Cobalt

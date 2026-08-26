// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Core/EditorState.hpp"
#include "Engine/Core/Application.hpp"
#include "Engine/Profiling/FrameProfiler.hpp"

namespace Cobalt
{
    class EditorApplication final : public Application
    {
    public:
        auto OnBegin() -> void override;
        auto OnUpdate() -> void override;
        auto OnDraw() -> void override;

    private:
        auto DrawMainDockSpace() -> void;
        auto DrawMainMenuBar() -> void;

    private:
        EditorState _state = {};
        FrameProfilerData _frame_profiler_data = {};
        f32 _profiler_update_time = 0.0f;
        bool _show_metrics_window = false;
    };
} // namespace Cobalt

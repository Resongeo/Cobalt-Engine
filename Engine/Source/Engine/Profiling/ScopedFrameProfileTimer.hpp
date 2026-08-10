// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"

#include <chrono>

namespace Cobalt
{
    class ScopedFrameProfileTimer final
    {
    public:
        explicit ScopedFrameProfileTimer(const String& name);
        ~ScopedFrameProfileTimer();

    private:
        String _name;
        std::chrono::steady_clock::time_point _start_time_point;
    };
}
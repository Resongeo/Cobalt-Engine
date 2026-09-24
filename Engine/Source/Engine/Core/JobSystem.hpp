// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Error.hpp"
#include <TaskScheduler.h>

namespace Cobalt
{
    class JobSystem final
    {
    public:
        auto Init() -> Result<bool, CoreInitError>;
        auto Shutdown() -> void;
        auto GetScheduler() -> enki::TaskScheduler&;

        static auto Get() -> JobSystem&;

    private:
        enki::TaskScheduler _scheduler;
    };
}
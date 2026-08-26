// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/JobSystem.hpp"
#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Log.hpp"

#include <rpmalloc.h>

namespace Cobalt
{
    auto JobSystem::Init() -> void {
        const auto thread_count = std::max(enki::GetNumHardwareThreads() - 2, 1u);

        auto config = enki::TaskSchedulerConfig{};
        config.numTaskThreadsToCreate = thread_count;
        config.profilerCallbacks.threadStart = [](u32) {
            rpmalloc_thread_initialize();
        };
        config.profilerCallbacks.threadStop = [](u32) {
            rpmalloc_thread_finalize();
        };

        _scheduler.Initialize(config);

        CORE_INFO("Job System initialized with {} threads", thread_count);
    }

    auto JobSystem::Shutdown() -> void {
        _scheduler.WaitforAllAndShutdown();
    }

    auto JobSystem::GetScheduler() -> enki::TaskScheduler& {
        return _scheduler;
    }

    auto JobSystem::Get() -> JobSystem& {
        static JobSystem instance;
        return instance;
    }
} // namespace Cobalt

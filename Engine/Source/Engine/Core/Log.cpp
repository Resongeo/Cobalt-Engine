// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Log.hpp"
#include "Engine/Core/EngineContext.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Cobalt
{
    Rc<spdlog::logger> Log::s_core_logger = nullptr;

    auto Log::init(EngineContext& ctx) -> void {
        s_core_logger = spdlog::stdout_color_mt("Engine");
    }

    auto Log::get_core_logger() -> Rc<spdlog::logger>& {
        return s_core_logger;
    }
} // namespace Cobalt

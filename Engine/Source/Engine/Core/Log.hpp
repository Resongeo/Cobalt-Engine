// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Memory.hpp"
#include "Engine/Core/Fwd.hpp"

#include <spdlog/spdlog.h>

namespace Cobalt
{
    using SinkPtr = std::shared_ptr<spdlog::logger>;

    class Log final
    {
    public:
        static auto Init() -> void;
        static auto FlushEvents() -> void;
        static auto CoreLogger() -> SinkPtr&;

    private:
        static SinkPtr _core_logger;
    };
} // namespace Cobalt

#define CORE_INFO(...)     ::Cobalt::Log::CoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::Cobalt::Log::CoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::Cobalt::Log::CoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::Cobalt::Log::CoreLogger()->critical(__VA_ARGS__)

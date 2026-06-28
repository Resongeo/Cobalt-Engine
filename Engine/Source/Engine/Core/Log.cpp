// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Log.hpp"
#include "Engine/Events/LogEvents.hpp"
#include "Engine/Events/EventBus.hpp"

#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Cobalt
{
    Rc<spdlog::logger> Log::_core_logger = nullptr;

    struct PendingLogEntry
    {
        spdlog::level::level_enum raw_level;
        String message;
    };

    static std::mutex log_queue_mutex;
    static Vector<PendingLogEntry> pending_logs;

    auto Log::Init() -> void {
        auto callback_sink = Memory::MakeRc<spdlog::sinks::callback_sink_mt>([&](const spdlog::details::log_msg& msg) {
            std::lock_guard lock(log_queue_mutex);
            pending_logs.push_back({msg.level, String(msg.payload.data(), msg.payload.size())});
        });
        auto console_sink = Memory::MakeRc<spdlog::sinks::stdout_color_sink_mt>();
        _core_logger = Memory::MakeRc<spdlog::logger>("Engine", spdlog::sinks_init_list{console_sink, callback_sink});
    }

    auto Log::FlushEvents() -> void {
        static Vector<PendingLogEntry> logs_to_process = {};

        {
            std::lock_guard lock(log_queue_mutex);
            if (!pending_logs.empty()) {
                logs_to_process.swap(pending_logs);
            } else {
                return;
            }
        }

        for (const auto& entry : logs_to_process) {
            auto cobalt_level = LogLevel::Info;
            switch (entry.raw_level) {
                case spdlog::level::warn: cobalt_level = LogLevel::Warn; break;
                case spdlog::level::err: cobalt_level = LogLevel::Error; break;
                case spdlog::level::critical: cobalt_level = LogLevel::Critical; break;
                default: cobalt_level = LogLevel::Info; break;
            }

            EventBus::Trigger<LogEvent>({.level = cobalt_level, .message = entry.message});
        }

        pending_logs.clear();
    }

    auto Log::CoreLogger() -> Rc<spdlog::logger>& {
        return _core_logger;
    }
} // namespace Cobalt

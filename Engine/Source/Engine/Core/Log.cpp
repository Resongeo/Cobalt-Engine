// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Core/Log.hpp"
#include "Engine/Core/EngineContext.hpp"
#include "Engine/Events/LogEvents.hpp"

#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Cobalt
{
    Rc<spdlog::logger> Log::s_core_logger = nullptr;

    struct PendingLogEntry
    {
        spdlog::level::level_enum raw_level;
        String message;
    };

    static std::mutex s_log_queue_mutex;
    static Vector<PendingLogEntry> s_pending_logs;

    auto Log::init(EngineContext& ctx) -> void {
        auto callback_sink = Memory::make_rc<spdlog::sinks::callback_sink_mt>([&](const spdlog::details::log_msg& msg) {
            std::lock_guard<std::mutex> lock(s_log_queue_mutex);
            s_pending_logs.push_back({msg.level, String(msg.payload.data(), msg.payload.size())});
        });
        auto console_sink = Memory::make_rc<spdlog::sinks::stdout_color_sink_mt>();
        s_core_logger = Memory::make_rc<spdlog::logger>("Engine", spdlog::sinks_init_list{console_sink, callback_sink});
    }

    auto Log::flush_events(EngineContext& ctx) -> void {
        static Vector<PendingLogEntry> logs_to_process = {};

        {
            std::lock_guard lock(s_log_queue_mutex);
            if (!s_pending_logs.empty()) {
                logs_to_process.swap(s_pending_logs);
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

            ctx.dispatcher.trigger<LogEvent>({.level = cobalt_level, .message = entry.message});
        }

        s_pending_logs.clear();
    }

    auto Log::get_core_logger() -> Rc<spdlog::logger>& {
        return s_core_logger;
    }
} // namespace Cobalt

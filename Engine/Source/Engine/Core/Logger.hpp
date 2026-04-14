// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Defines.hpp"
#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"

#include <chrono>
#include <ctime>
#include <format>
#include <iostream>

namespace Cobalt
{
    enum class LogLevel : u8
    {
        Trace,
        Warn,
        Error,
        Fatal,
    };

    // TODO: Make thread safe
    class Logger final
    {
    public:
        template <typename... Args>
        static auto trace(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            _log(LogLevel::Trace, location, msg);
        }

        template <typename... Args>
        static auto warn(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            _log(LogLevel::Warn, location, msg);
        }

        template <typename... Args>
        static auto error(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            _log(LogLevel::Error, location, msg);
        }

        template <typename... Args>
        static auto fatal(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            _log(LogLevel::Fatal, location, msg);
        }

    private:
        static auto _log(const LogLevel level, const String& loc, const String& msg) -> void {
            const auto time_str = _timestamp();
            const auto level_str = _log_level(level);
            const auto color_str = _log_color(level);

            constexpr auto RESET = "\033[0m";
            constexpr auto GRAY = "\033[37m";

            const auto final_msg =
                    std::format("{}({}) {}[{} - {}]{} {}", GRAY, time_str, color_str, level_str, loc, RESET, msg);

            std::cout << final_msg << "\n";
        }

        static auto _timestamp() -> String {
            const auto now = std::chrono::system_clock::now();
            const auto time = std::chrono::system_clock::to_time_t(now);
            auto tm = std::tm{};

#ifdef PLATFORM_LINUX
            localtime_r(&time, &tm);
#elifdef PLATFORM_WINDOWS
            localtime_s(&tm, &time);
#endif

            std::ostringstream oss;
            oss << std::put_time(&tm, "%H:%M:%S");
            return oss.str();
        }

        static auto _log_level(const LogLevel level) -> String {
            switch (level) {
                case LogLevel::Trace: return "TRACE";
                case LogLevel::Warn: return "WARN";
                case LogLevel::Error: return "ERROR";
                case LogLevel::Fatal: return "FATAL";
            }
        }

        static auto _log_color(const LogLevel level) -> String {
            switch (level) {
                case LogLevel::Trace: return "\033[94m";
                case LogLevel::Warn: return "\033[93m";
                case LogLevel::Error: return "\033[91m";
                case LogLevel::Fatal: return "\033[31m";
            }
        }
    };
} // namespace Cobalt

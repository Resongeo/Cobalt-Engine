// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types.hpp"
#include <format>
#include <chrono>
#include <ctime>
#include <iostream>

namespace Cobalt::Engine
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
        template<typename... Args>
        static auto trace(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            log(LogLevel::Trace, location, msg);
        }

        template<typename... Args>
        static auto warn(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            log(LogLevel::Warn, location, msg);
        }

        template<typename... Args>
        static auto error(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            log(LogLevel::Error, location, msg);
        }

        template<typename... Args>
        static auto fatal(const String& location, std::format_string<Args...> fmt, Args&&... args) -> void {
            auto msg = std::format(fmt, std::forward<Args>(args)...);
            log(LogLevel::Fatal, location, msg);
        }

    private:
        static auto log(const LogLevel level, const String& loc, const String& msg) -> void {
            const auto time_str = timestamp();
            const auto level_str = log_level(level);
            const auto color_str = log_color(level);

            constexpr auto RESET = "\033[0m";
            constexpr auto GRAY = "\033[37m";

            const auto final_msg = std::format("({}) {}[{} - {}]{} {}{}",
                time_str,
                color_str,
                level_str,
                loc,
                GRAY,
                msg,
                RESET
            );

            std::cout << final_msg << "\n";
        }

        static auto timestamp() -> String {
            const auto now = std::chrono::system_clock::now();
            const auto time = std::chrono::system_clock::to_time_t(now);
            auto tm = std::tm{};

            // TODO: Make this cross platform
            localtime_r(&time, &tm);

            std::ostringstream oss;
            oss << std::put_time(&tm, "%H:%M:%S");
            return oss.str();
        }

        static auto log_level(const LogLevel level) -> String {
            switch (level) {
                case LogLevel::Trace: return "TRACE";
                case LogLevel::Warn: return "WARN";
                case LogLevel::Error: return "ERROR";
                case LogLevel::Fatal: return "FATAL";
            }
        }

        static auto log_color(const LogLevel level) -> String {
            switch (level) {
                case LogLevel::Trace: return "\033[94m";
                case LogLevel::Warn: return "\033[93m";
                case LogLevel::Error: return "\033[91m";
                case LogLevel::Fatal: return "\033[31m";
            }
        }
    };
}

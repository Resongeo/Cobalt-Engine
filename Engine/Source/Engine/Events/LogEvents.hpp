// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt
{
    enum class LogLevel
    {
        Info = 0,
        Warn,
        Error,
        Critical,
    };

    struct LogEvent
    {
        LogLevel level = LogLevel::Info;
        String message = {};
    };
}
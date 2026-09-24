// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Defines.hpp"
#include "Engine/Core/Log.hpp"
#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Base.hpp"

namespace Cobalt
{
    enum class CoreInitError
    {
        ProjectFileDoesntExists,
        ProjectFileCantParse,
        ProjectFileNotValid,
        PlatformSDL3Init,
        PlatformCreateWindow,
        PlatformCreateOpenGLContext,
        ScriptCreateEngine,
        ScriptCreateContext,
        ScriptSetMessageCallback,
        ScriptRegisterInterface,
        SIZE,
    };

    static inline Array<String, static_cast<usize>(CoreInitError::SIZE)> CoreInitErrorStr = {
        "ProjectFileDoesntExists",
        "ProjectFileCantParse",
        "ProjectFileNotValid",
        "PlatformSDL3Init",
        "PlatformCreateWindow",
        "PlatformCreateOpenGLContext",
        "ScriptCreateEngine",
        "ScriptCreateContext",
        "ScriptSetMessageCallback",
        "ScriptRegisterInterface",
    };

#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__clang__)
#define DEBUG_BREAK() __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#define DEBUG_BREAK() __asm__ volatile("int $0x03")
#else
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#endif

#if defined(CONFIGURATION_DEBUG)
#define CORE_ASSERT(condition, ...)                                                                                                        \
    do {                                                                                                                                   \
        if (!(condition)) {                                                                                                                \
            CORE_CRITICAL("Assertion Failed: {}", __VA_ARGS__);                                                                            \
            DEBUG_BREAK();                                                                                                                 \
        }                                                                                                                                  \
    } while (false)
#else
#define CORE_ASSERT(condition, ...) ((void)0)
#endif

#define TRY(expr)                                                                                                                          \
    do {                                                                                                                                   \
        auto _res = (expr);                                                                                                                \
        if (!_res) return Err(_res.error());                                                                                               \
    } while (false)
} // namespace Cobalt

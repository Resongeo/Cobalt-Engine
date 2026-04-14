// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Application.hpp"
#include "Engine/Core/CommandLineArgs.hpp"
#include "Engine/Core/Defines.hpp"
#include "Engine/Core/Types/Memory.hpp"

namespace Cobalt
{
    extern auto create_application(CommandLineArgs args) -> Box<Application>;
} // namespace Cobalt

#if defined(PLATFORM_WINDOWS) && defined(CONFIGURATION_RELEASE)

#include <Windows.h>
auto WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) -> int {
    const auto app = Cobalt::create_application({__argc, __argv});
    app->run();
}

#else

auto main(const int argc, char* argv[]) -> int {
    const auto app = Cobalt::create_application({argc, argv});
    app->run();
}

#endif

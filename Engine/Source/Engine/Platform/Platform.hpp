// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Platform/Window.hpp"

namespace Cobalt::Engine::Platform
{
    auto initialize() -> void;
    auto opengl_init() -> void;
    auto destroy() -> void;
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

namespace Cobalt::Platform
{
    auto sdl3_init() -> bool;
    auto opengl_init() -> bool;
    auto destroy() -> void;
} // namespace Cobalt::Platform

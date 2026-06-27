// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

namespace Cobalt::Platform
{
    auto InitSDL3() -> bool;
    auto InitOpenGL() -> bool;
    auto ShutDown() -> void;
} // namespace Cobalt::Platform

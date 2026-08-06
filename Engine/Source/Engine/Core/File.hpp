// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt
{
    class File final
    {
    public:
        static auto Read(const Filepath& path) -> String;
        static auto Save(const Filepath& path, const String& content) -> bool;
        static auto Exists(const Filepath& path) -> bool;
    };
} // namespace Cobalt

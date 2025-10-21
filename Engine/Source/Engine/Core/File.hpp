// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt::Engine
{
    class File final
    {
    public:
        static auto read(const Filepath& path) -> String;
        static auto exists(const Filepath& path) -> bool;
    };
}
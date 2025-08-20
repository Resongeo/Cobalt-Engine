// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace Cobalt
{
    using String = std::string;
    using Filepath = std::filesystem::path;

    template<typename T>
    using Vector = std::vector<T>;
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <initializer_list>

namespace Cobalt
{
    using String = std::string;
    using Filepath = std::filesystem::path;

    template<typename T>
    using Vector = std::vector<T>;

    template<typename T>
    using InitializerList = std::initializer_list<T>;

    template<typename K, typename V>
    using HashMap = std::unordered_map<K, V>;
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <EASTL/initializer_list.h>
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <filesystem>

namespace Cobalt
{
    using String = std::string;
    using StringView = std::string_view;
    using Filepath = std::filesystem::path;

    template <typename T>
    using Vector = eastl::vector<T>;

    template <typename T>
    using InitializerList = std::initializer_list<T>;

    template <typename K, typename V>
    using HashMap = eastl::unordered_map<K, V>;
} // namespace Cobalt

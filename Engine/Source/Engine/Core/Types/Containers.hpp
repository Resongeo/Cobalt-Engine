// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <array>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <EASTL/array.h>
#include <EASTL/hash_map.h>
#include <EASTL/span.h>
#include <EASTL/string.h>
#include <EASTL/string_view.h>
#include <EASTL/vector.h>

namespace Cobalt
{
    using String = eastl::basic_string<char>;
    using StringView = eastl::basic_string_view<char>;
    using Filepath = std::filesystem::path;

    template <typename T>
    using Vector = eastl::vector<T>;
    template <typename T, size_t S>
    using Array = eastl::array<T, S>;
    template <typename T>
    using Span = eastl::span<T>;

    template <typename T>
    using InitializerList = std::initializer_list<T>;

    template <typename K, typename V>
    using HashMap = eastl::hash_map<K, V>;

    template <typename T>
    using Optional = std::optional<T>;
    inline constexpr auto None = std::nullopt;
} // namespace Cobalt

// std::filesystem::path hashing needed for eastl::hash_map
namespace eastl
{
    template <>
    struct hash<std::filesystem::path>
    {
        size_t operator()(const std::filesystem::path& path) const noexcept {
            return std::hash<std::string_view>{}(path.string());
        }
    };
} // namespace eastl

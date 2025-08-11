// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>
#include <random>

namespace Cobalt
{
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;
    typedef int8_t   i8;
    typedef int16_t  i16;
    typedef int32_t  i32;
    typedef int64_t  i64;
    typedef size_t   usize;
    typedef float    f32;
    typedef double   f64;

    using String = std::string;
    using Filepath = std::filesystem::path;

    template<typename T>
    using Vector = std::vector<T>;

    struct UUID {
        u64 value;

        explicit UUID() : value(0) {}
        explicit UUID(const u64 val) : value(val) {}

        auto is_valid() const -> bool {
            return value != 0;
        }

        static auto generate() -> UUID {
            static std::random_device rd;
            static std::mt19937_64 gen(rd());
            return UUID(gen());
        }

        auto operator==(const UUID& other) const -> bool {
            return value == other.value;
        }
        auto operator!=(const UUID& other) const -> bool {
            return value != other.value;
        }
    };
}

/* UUID hashing needed for std::unordered_map */
namespace std {
    template<> struct hash<Cobalt::UUID> {
        auto operator()(const Cobalt::UUID& id) const -> Cobalt::usize {
            return hash<uint64_t>()(id.value);
        }
    };
}
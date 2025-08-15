// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>
#include <random>
#include <memory>

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

    template<typename T>
    using Unique = std::unique_ptr<T>;

    // TODO: Implement custom smart pointers
    template<typename T>
    using Box = std::unique_ptr<T>;

    template<typename T>
    using Rc = std::shared_ptr<T>;

    namespace Memory
    {
        template<typename T, typename ... Args>
        constexpr auto make_box(Args&& ... args) -> Box<T> {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }

        template<typename T, typename ... Args>
        constexpr auto make_rc(Args&& ... args) -> Rc<T> {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    }

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
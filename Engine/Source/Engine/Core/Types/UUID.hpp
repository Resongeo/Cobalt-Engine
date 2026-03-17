// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include <random>

namespace Cobalt
{
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
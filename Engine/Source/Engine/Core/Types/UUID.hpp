// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Base.hpp"
#include "Engine/Core/Types/Containers.hpp"

#include <random>

namespace Cobalt
{
    struct UUID
    {
        u64 value;

        UUID() : value(0) {}
        explicit UUID(const u64 val) : value(val) {}
        explicit UUID(const String& val) {
            const auto str = val.c_str();
            char* end;
            value = strtoll(str, &end, 10);
        }

        auto IsValid() const -> bool {
            return value != 0;
        }

        static auto Generate() -> UUID {
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

        operator u64() {
            return value;
        }

        operator u64() const {
            return value;
        }
    };
} // namespace Cobalt

// UUID hashing needed for eastl::hash_map
namespace eastl
{
    template <>
    struct hash<Cobalt::UUID>
    {
        size_t operator()(const Cobalt::UUID& uuid) const {
            return uuid.value;
        }
    };
} // namespace eastl

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Types.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("UUID") {
    SECTION("Default constructor") {
        const auto id = Cobalt::UUID();
        REQUIRE(id.value == 0);
        REQUIRE(id.is_valid() == false);
    }

    SECTION("Generate function") {
        const auto id = Cobalt::UUID::generate();
        REQUIRE(id.value != 0);
        REQUIRE(id.is_valid() == true);
    }
}

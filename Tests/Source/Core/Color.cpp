// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Core/Types/Color.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Color") {
    SECTION("From Scalar f32") {
        constexpr float expected = 1.0f;
        const auto [r, g, b, a] = Cobalt::Color::from_scalar(1.0f);
        REQUIRE(r == expected);
        REQUIRE(g == expected);
        REQUIRE(b == expected);
        REQUIRE(a == expected);
    }

    SECTION("From Scalar u8") {
        constexpr float expected = 1.0f;
        const auto [r, g, b, a] = Cobalt::Color::from_scalar(static_cast<uint8_t>(255));
        REQUIRE(r == expected);
        REQUIRE(g == expected);
        REQUIRE(b == expected);
        REQUIRE(a == expected);
    }

    SECTION("From hex") {
        const auto red = Cobalt::Color::from_hex(0xff0000);
        REQUIRE(red.r == 1.0f);
        REQUIRE(red.g == 0.0f);
        REQUIRE(red.b == 0.0f);
        REQUIRE(red.a == 1.0f);

        const auto green = Cobalt::Color::from_hex(0x00ff00);
        REQUIRE(green.r == 0.0f);
        REQUIRE(green.g == 1.0f);
        REQUIRE(green.b == 0.0f);
        REQUIRE(green.a == 1.0f);

        const auto white = Cobalt::Color::from_hex(0xffffff);
        REQUIRE(white.r == 1.0f);
        REQUIRE(white.g == 1.0f);
        REQUIRE(white.b == 1.0f);
        REQUIRE(white.a == 1.0f);
    }

    SECTION("From oklch") {
        const auto black = Cobalt::Color::from_oklch(0.0f, 0.0f, 0.0f);
        REQUIRE(black.r == 0.0f);
        REQUIRE(black.g == 0.0f);
        REQUIRE(black.b == 0.0f);
        REQUIRE(black.a == 1.0f);

        // Degree does not matter here since lightness if full
        const auto white = Cobalt::Color::from_oklch(1.0f, 0.0f, 235.0f, 0.5f);
        // Take into some floating errors
        REQUIRE(white.r >= 0.999f);
        REQUIRE(white.g >= 0.999f);
        REQUIRE(white.b >= 0.999f);
        REQUIRE(white.a == 0.5f);
    }
}

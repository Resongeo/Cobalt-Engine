// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"

#include <algorithm>
#include <cmath>

namespace Cobalt
{
    struct Color
    {
        f32 r = 1.0f;
        f32 g = 1.0f;
        f32 b = 1.0f;
        f32 a = 1.0f;

        static auto from_scalar(const f32 value, const f32 alpha = 1.0f) -> Color {
            return {value, value, value, alpha};
        }

        static auto from_rgb(const f32 r, const f32 g, const f32 b) -> Color {
            return {r, g, b, 1.0};
        }

        static auto from_rgba(const f32 r, const f32 g, const f32 b, const f32 a) -> Color {
            return {r, g, b, a};
        }

        static auto from_scalar(const u8 value, const u8 alpha = 255) -> Color {
            return {
                static_cast<f32>(value) / 255.0f,
                static_cast<f32>(value) / 255.0f,
                static_cast<f32>(value) / 255.0f,
                static_cast<f32>(alpha) / 255.0f,
            };
        }

        static auto from_rgb(const u8 r, const u8 g, const u8 b) -> Color {
            return {
                static_cast<f32>(r) / 255.0f,
                static_cast<f32>(g) / 255.0f,
                static_cast<f32>(b) / 255.0f,
                1.0f,
            };
        }

        static auto from_rgba(const u8 r, const u8 g, const u8 b, const u8 a) -> Color {
            return {
                static_cast<f32>(r) / 255.0f,
                static_cast<f32>(g) / 255.0f,
                static_cast<f32>(b) / 255.0f,
                static_cast<f32>(a) / 255.0f,
            };
        }

        static auto from_hex(const u32 hex) -> Color {
            // TODO: Handle alpha values in hex
            return {
                (hex >> 16 & 0xFF) / 255.0f,
                (hex >> 8 & 0xFF) / 255.0f,
                (hex & 0xFF) / 255.0f,
                1.0f
            };
        }

        static auto from_oklch(f32 l, f32 c, f32 deg, f32 alpha = 1.0f) -> Color {
            constexpr auto PI = 3.14159265358979323846;

            // OKLCH -> OKLab
            const auto _a = c * std::cos(deg * PI / 180.0f);
            const auto _b = c * std::sin(deg * PI / 180.0f);

            const auto l_oklab = l;
            const auto a_oklab = _a;
            const auto b_oklab = _b;

            // OKLab -> LMS cube roots
            auto _l = l_oklab + 0.3963377774f * a_oklab + 0.2158037573f * b_oklab;
            const auto _m = l_oklab - 0.1055613458f * a_oklab - 0.0638541728f * b_oklab;
            const auto _s = l_oklab - 0.0894841775f * a_oklab - 1.2914855480f * b_oklab;

            // Undo cube root
            _l = _l * _l * _l;
            const auto m = _m * _m * _m;
            const auto s = _s * _s * _s;

            // LMS -> sRGB
            const auto r_lin = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
            const auto g_lin = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
            const auto b_lin = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

            // sRGB -> gamma encoded
            auto srgb_transfer = [](const f32 x) {
                if (x <= 0.0031308f) {
                    return 12.92f * x;
                }
                return 1.055f * std::pow(x, 1.0f / 2.4f) - 0.055f;
            };

            auto r = srgb_transfer(r_lin);
            auto g = srgb_transfer(g_lin);
            auto b = srgb_transfer(b_lin);

            r = std::clamp(r, 0.0f, 1.0f);
            g = std::clamp(g, 0.0f, 1.0f);
            b = std::clamp(b, 0.0f, 1.0f);

            return {r, g, b, alpha};
        }

        operator Vec4() {
            return {r, g, b, a};
        }
    };
}
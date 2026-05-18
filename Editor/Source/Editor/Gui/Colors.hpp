// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Color.hpp"

#define IMVEC4(col) ImVec4(col.r, col.g, col.b, col.a)

namespace Cobalt::Colors
{
    inline Color text       = Color::from_oklch(0.90f, 0.0f, 0.0f);
    inline Color text_muted = Color::from_oklch(0.50f, 0.0f, 0.0f);

    inline Color frame         = Color::from_oklch(0.25f, 0.0f, 0.0f);
    inline Color frame_hovered = Color::from_oklch(0.28f, 0.0f, 0.0f);
    inline Color frame_active  = Color::from_oklch(0.32f, 0.0f, 0.0f);

    inline Color primary         = Color::from_hex(0x2F69D4);
    inline Color primary_hovered = Color::from_hex(0x2352A9);
    inline Color primary_active  = Color::from_hex(0x1A3D7F);

    inline Color secondary         = Color::from_hex(0xD42B2B);
    inline Color secondary_hovered = Color::from_hex(0xA92323);
    inline Color secondary_active  = Color::from_hex(0x7F1A1A);

    inline Color node_even    = Color::from_oklch(0.22f, 0.0f, 0.0f);
    inline Color node_odd     = Color::from_oklch(0.24f, 0.0f, 0.0f);
    inline Color node_active  = primary_active;
    inline Color node_hovered = primary_hovered;

    inline Color arrow     = Color::from_oklch(0.65f, 0.0f, 0.0f);
    inline Color tag       = Color::from_rgb_u8(88, 153, 232);
    inline Color transform = Color::from_rgb_u8(232, 150, 88);
    inline Color sprite    = Color::from_rgb_u8(195, 119, 230);
    inline Color script    = Color::from_rgb_u8(72, 115, 217);
    inline Color directory = Color::from_rgb_u8(194, 159, 83);
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace Cobalt
{
    enum class Variant
    {
        Default = 0,
        Primary,
        Secondary
    };

    namespace Widgets
    {
        auto begin(StringView title, ImVec2 padding = {0, 0}) -> bool;
        auto end() -> void;

        auto separator() -> void;
        auto button(StringView label, Variant variant = Variant::Default, ImVec2 size = {0, 0}) -> bool;
    }
}
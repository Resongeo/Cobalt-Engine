// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include <imgui.h>

namespace Cobalt
{
    class ScopedStyleColor
    {
    public:
        ScopedStyleColor(const ImGuiCol idx, const ImU32 color) { ImGui::PushStyleColor(idx, color); }
        ScopedStyleColor(const ImGuiCol idx, const ImVec4& color) { ImGui::PushStyleColor(idx, color); }
        ~ScopedStyleColor() { ImGui::PopStyleColor(); }
    };
    class ScopedStyleVar
    {
    public:
        ScopedStyleVar(const ImGuiStyleVar idx, const float value) { ImGui::PushStyleVar(idx, value); }
        ScopedStyleVar(const ImGuiStyleVar idx, const ImVec2& value) { ImGui::PushStyleVar(idx, value); }
        ~ScopedStyleVar() { ImGui::PopStyleVar(); }
    };

#define CAT_(a, b) a ## b
#define CAT(a, b) CAT_(a, b)
#define VARNAME(Var) CAT(Var, __LINE__)

#define SCOPED_STYLE_COLOR(...) ScopedStyleColor VARNAME(scopedStyleColor)(__VA_ARGS__)
#define SCOPED_STYLE_VAR(...) ScopedStyleVar VARNAME(scopedStyleVar)(__VA_ARGS__)
}
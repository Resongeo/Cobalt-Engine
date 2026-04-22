// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/Fonts.hpp"

#include <imgui_internal.h>

namespace Cobalt::Widgets
{
    auto begin(const StringView title, const ImVec2 padding) -> bool {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
        const auto result = ImGui::Begin(title.data());
        ImGui::PopStyleVar();

        return result;
    }

    auto end() -> void {
        ImGui::End();
    }

    auto separator() -> void {
        ImGui::Separator();
    }

    auto button(const StringView label, Variant variant, const ImVec2 size) -> bool {
        const auto window = ImGui::GetCurrentWindow();
        if (window->SkipItems) {
            return false;
        }

        ImGui::PushFont(Fonts::semibold);

        const auto& g = *GImGui;
        const auto& style = g.Style;
        const auto id = window->GetID(label.data());
        const auto label_size = ImGui::CalcTextSize(label.data(), nullptr, true);

        auto pos = window->DC.CursorPos;
        if (style.FramePadding.y < window->DC.CurrLineTextBaseOffset) {
            // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit
            // hacky, since it shouldn't be a flag)
            pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
        }
        const auto button_size = ImGui::CalcItemSize(size, label_size.x + style.FramePadding.x * 2.0f,
                                                     label_size.y + style.FramePadding.y * 2.0f);

        const auto bb = ImRect(pos, pos + button_size);
        ImGui::ItemSize(button_size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id)) {
            ImGui::PopFont();
            return false;
        }

        bool hovered, held;
        const bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

        const ImU32 col = ImGui::GetColorU32(held && hovered   ? ImGuiCol_ButtonActive
                                                     : hovered ? ImGuiCol_ButtonHovered
                                                               : ImGuiCol_Button);
        ImGui::RenderNavCursor(bb, id);
        ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

        ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label.data(), nullptr,
                                 &label_size, style.ButtonTextAlign, &bb);

        ImGui::PopFont();

        return pressed;
    }
} // namespace Cobalt::Widgets

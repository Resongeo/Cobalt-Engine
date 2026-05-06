// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/Colors.hpp"
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

    auto button(const StringView label, const Variant variant, const ImVec2 size) -> bool {
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
        auto bg_col = Color{};

        switch (variant) {
            case Variant::Default: {
                bg_col = held && hovered ? Colors::frame_active : hovered ? Colors::frame_hovered : Colors::frame;
                break;
            }
            case Variant::Primary: {
                bg_col = held && hovered ? Colors::primary_active : hovered ? Colors::primary_hovered : Colors::primary;
                break;
            }
            case Variant::Secondary: {
                bg_col = held && hovered ? Colors::secondary_active
                        : hovered        ? Colors::secondary_hovered
                                         : Colors::secondary;
                break;
            }
        }

        const auto col = ImGui::GetColorU32(IMVEC4(bg_col));
        ImGui::RenderNavCursor(bb, id);
        ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

        ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label.data(), nullptr,
                                 &label_size, style.ButtonTextAlign, &bb);

        ImGui::PopFont();

        return pressed;
    }

    auto collapsing_header(const StringView label, const Color& text_color, const bool default_opened) -> bool {
        auto* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) {
            return false;
        }

        ImGui::PushFont(Fonts::semibold);

        const auto id = window->GetID(label.data());

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_CollapsingHeader;
        if (default_opened) {
            flags = flags | ImGuiTreeNodeFlags_DefaultOpen;
        }

        auto& g = *GImGui;
        const auto& style = g.Style;

        const auto padding = style.FramePadding;
        const char* label_end = ImGui::FindRenderedTextEnd(label.data());
        const auto label_size = ImGui::CalcTextSize(label.data(), label_end, false);

        const auto text_offset_x = g.FontSize + padding.x; // Collapsing arrow width + Spacing
        const auto text_offset_y =
                ImMax(style.FramePadding.y, window->DC.CurrLineTextBaseOffset); // Latch before ItemSize changes it
        const auto text_width = g.FontSize + label_size.x + padding.x * 2; // Include collapsing arrow
        const auto frame_height = label_size.y + padding.y * 2;

        ImRect frame_bb;
        frame_bb.Min.x = flags & ImGuiTreeNodeFlags_SpanFullWidth ? window->WorkRect.Min.x : window->DC.CursorPos.x;
        frame_bb.Min.y = window->DC.CursorPos.y + (text_offset_y - padding.y);
        frame_bb.Max.x = flags & ImGuiTreeNodeFlags_SpanLabelWidth ? window->DC.CursorPos.x + text_width + padding.x
                                                                   : window->WorkRect.Max.x;
        frame_bb.Max.y = window->DC.CursorPos.y + (text_offset_y - padding.y) + frame_height;

        // -- Extend frame to take up all space available --
        const float outer_extend = IM_TRUNC(window->WindowPadding.x);
        frame_bb.Min.x -= outer_extend;
        frame_bb.Max.x += outer_extend;

        const auto text_pos = ImVec2(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
        ImGui::ItemSize(ImVec2(text_width, frame_height), padding.y);

        // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
        auto interact_bb = frame_bb;
        if ((flags &
             (ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth |
              ImGuiTreeNodeFlags_SpanLabelWidth | ImGuiTreeNodeFlags_SpanAllColumns)) == 0) {
            interact_bb.Max.x = frame_bb.Min.x + text_width + (label_size.x > 0.0f ? style.ItemSpacing.x * 2.0f : 0.0f);
        }

        // Compute open and multi-select states before ItemAdd() as it clear NextItem data.
        const auto storage_id =
                g.NextItemData.HasFlags & ImGuiNextItemDataFlags_HasStorageID ? g.NextItemData.StorageId : id;
        bool is_open = ImGui::TreeNodeUpdateNextOpen(storage_id, flags);

        const bool is_visible = ImGui::ItemAdd(interact_bb, id);
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
        g.LastItemData.DisplayRect = frame_bb;

        // If a NavLeft request is happening and ImGuiTreeNodeFlags_NavLeftJumpsToParent enabled:
        // Store data for the current depth to allow returning to this node from any child item.
        // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
        // It will become tempting to enable ImGuiTreeNodeFlags_NavLeftJumpsToParent by default or move it to
        // ImGuiStyle.

        if (!is_visible) {
            if (flags & ImGuiTreeNodeFlags_DrawLinesToNodes &&
                window->DC.TreeRecordsClippedNodesY2Mask & 1 << (window->DC.TreeDepth - 1)) {
                ImGuiTreeNodeStackData* parent_data = &g.TreeNodeStack.Data[g.TreeNodeStack.Size - 1];
                parent_data->DrawLinesToNodesY2 =
                        ImMax(parent_data->DrawLinesToNodesY2,
                              window->DC.CursorPos.y); // Don't need to aim to mid Y position as we are clipped anyway.

                if (frame_bb.Min.y >= window->ClipRect.Max.y) {
                    window->DC.TreeRecordsClippedNodesY2Mask &= ~(1 << (window->DC.TreeDepth - 1)); // Done
                }
            }

            if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
                ImGui::TreePushOverrideID(id);
            }

            ImGui::PopFont();
            return is_open;
        }

        ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
        if (flags & ImGuiTreeNodeFlags_AllowOverlap || g.LastItemData.ItemFlags & ImGuiItemFlags_AllowOverlap) {
            button_flags |= ImGuiButtonFlags_AllowOverlap;
        }

        // We allow clicking on the arrow section with keyboard modifiers held, in order to easily
        // allow browsing a tree while preserving selection with code implementing multi-selection patterns.
        // When clicking on the rest of the tree node we always disallow keyboard modifiers.
        const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
        const float arrow_hit_x2 =
                (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
        const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

        // Open behaviors can be altered with the _OpenOnArrow and _OnOnDoubleClick flags.
        // Some alteration have subtle effects (e.g. toggle on MouseUp vs MouseDown events) due to requirements for
        // multi-selection and drag and drop support.
        // - Single-click on label = Toggle on MouseUp (default, when _OpenOnArrow=0)
        // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=0)
        // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=1)
        // - Double-click on label = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1)
        // - Double-click on arrow = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1 and _OpenOnArrow=0)
        // It is rather standard that arrow click react on Down rather than Up.
        // We set ImGuiButtonFlags_PressedOnClickRelease on OpenOnDoubleClick because we want the item to be active on
        // the initial MouseDown in order for drag and drop to work.
        if (is_mouse_x_over_arrow) {
            button_flags |= ImGuiButtonFlags_PressedOnClick;
        } else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) {
            button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
        } else {
            button_flags |= ImGuiButtonFlags_PressedOnClickRelease;
        }

        if (flags & ImGuiTreeNodeFlags_NoNavFocus) {
            button_flags |= ImGuiButtonFlags_NoNavFocus;
        }

        const bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
        const bool was_selected = selected;

        if (window != g.HoveredWindow || !is_mouse_x_over_arrow) {
            button_flags |= ImGuiButtonFlags_NoKeyModsAllowed;
        }

        bool hovered, held;
        const bool pressed = ImGui::ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
        bool toggled = false;
        if (pressed && g.DragDropHoldJustPressedId != id) {
            if ((flags & ImGuiTreeNodeFlags_OpenOnMask_) == 0 || g.NavActivateId == id) {
                toggled = true; // Single click
            }

            if (flags & ImGuiTreeNodeFlags_OpenOnArrow) {
                // Lightweight equivalent of IsMouseHoveringRect() since
                // ButtonBehavior() already did the job
                toggled |= is_mouse_x_over_arrow && !g.NavHighlightItemUnderNav;
            }

            if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseClickedCount[0] == 2) {
                toggled = true; // Double click
            }
        } else if (pressed && g.DragDropHoldJustPressedId == id) {
            IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
            if (!is_open) {
                // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but
                // never close it again.
                toggled = true;
            }
        }

        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Left && is_open) {
            toggled = true;
            ImGui::NavClearPreferredPosForAxis(ImGuiAxis_X);
            ImGui::NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveDir == ImGuiDir_Right && !is_open) {
            // If there's something upcoming on the line we may want to give it the priority?
            toggled = true;
            ImGui::NavClearPreferredPosForAxis(ImGuiAxis_X);
            ImGui::NavMoveRequestCancel();
        }

        if (toggled) {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(storage_id, is_open);
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
        }

        if (selected != was_selected) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

        // -- Rendering part --
        constexpr ImGuiNavRenderCursorFlags nav_render_cursor_flags = ImGuiNavRenderCursorFlags_Compact;
        const auto bg_col = held && hovered ? Colors::frame_active : hovered ? Colors::frame_hovered : Colors::frame;

        ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(IMVEC4(bg_col)), false);
        ImGui::RenderNavCursor(frame_bb, id, nav_render_cursor_flags);

        ImGui::RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x, text_pos.y),
                           ImGui::GetColorU32(IMVEC4(Colors::arrow)),
                           is_open ? ((flags & ImGuiTreeNodeFlags_UpsideDownArrow) ? ImGuiDir_Up : ImGuiDir_Down)
                                   : ImGuiDir_Right,
                           1.0f);

        // -- Label --
        ImGui::PushStyleColor(ImGuiCol_Text, IMVEC4(text_color));
        ImGui::RenderTextClipped(text_pos, frame_bb.Max, label.data(), label_end, &label_size);
        ImGui::PopStyleColor();

        ImGui::PopFont();

        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
            ImGui::TreePushOverrideID(id); // Could use TreePush(label) but this avoid computing twice
        }

        return is_open;
    }
} // namespace Cobalt::Widgets

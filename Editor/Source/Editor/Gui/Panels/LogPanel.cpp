// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Gui/Panels/LogPanel.hpp"
#include "Editor/Gui/Widgets.hpp"
#include "Editor/Gui/FontIcons.hpp"
#include "Editor/Gui/Colors.hpp"
#include "Editor/Gui/Fonts.hpp"

#include <imgui_internal.h>

namespace Cobalt
{
    void LogPanel::begin(EngineContext& ctx, EditorState& state) {
        ctx.dispatcher.sink<LogEvent>().connect<&LogPanel::log_event>(this);
    }

    auto LogPanel::draw(EngineContext& ctx, EditorState& state) -> void {
        Widgets::begin("Logs", {8, 8});
        {
            if (Widgets::button(ICON_TRASH, Variant::Default, {0, 0}, true)) {
                m_log_entries.clear();
            }
            ImGui::SameLine();
            ImGui::Checkbox("Scroll to bottom", &m_scroll_to_bottom);
            ImGui::SameLine();
            ImGui::TextDisabled("Entries: %zu", m_log_entries.size());

            Widgets::separator();

            ImGui::BeginChild("Logs Region", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

            //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 3));

            auto index = 0;
            for (const auto& entry : m_log_entries) {
                auto color = ImVec4{};
                auto icon = "";

                switch (entry.level) {
                    case LogLevel::Warn:
                        color = ImVec4(0.9f, 0.7f, 0.2f, 1.0f);
                        icon = ICON_WARN;
                        break;
                    case LogLevel::Error:
                        color = ImVec4(0.9f, 0.3f, 0.3f, 1.0f);
                        icon = ICON_ERROR;
                        break;
                    case LogLevel::Critical:
                        color = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
                        icon = ICON_CRITICAL;
                        break;
                    case LogLevel::Info:
                    default:
                        color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
                        icon = ICON_INFO;
                        break;
                }


                constexpr auto padding = 6.0f;
                const auto content_avail = ImGui::GetContentRegionAvail();
                const auto title_height = ImGui::GetTextLineHeightWithSpacing();
                const auto min = ImGui::GetCursorScreenPos();
                const auto max = ImVec2{min.x + content_avail.x, min.y + title_height + padding * 2.0f};
                const auto size = max - min;

                auto bg_col = index++ & 1 ? Colors::node_odd : Colors::node_even;

                ImGui::PushID(index * 8127812 % 23);
                ImGui::InvisibleButton("##Node", size);
                const auto hovered = ImGui::IsItemHovered();
                ImGui::PopID();

                bg_col = hovered ? Colors::node_hovered : bg_col;

                ImGui::RenderFrame(min, max, ImGui::GetColorU32(IMVEC4(bg_col)), false);

                ImGui::SetCursorScreenPos({min.x + padding, min.y + padding});
                ImGui::PushFont(Fonts::icon);
                ImGui::TextColored(color, "%s\t", icon);
                ImGui::PopFont();
                ImGui::SameLine();
                ImGui::TextColored(color, "%s", entry.message.c_str());

                ImGui::SetCursorScreenPos(min);
                ImGui::Dummy(size);
            }

            if (m_scroll_to_bottom && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 10.0f) {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();
        }
        Widgets::end();
    }

    auto LogPanel::log_event(const LogEvent& event) -> void {
        m_log_entries.push_back(event);
    }
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Editor/Gui/IPanel.hpp"
#include "Engine/Events/LogEvents.hpp"

namespace Cobalt
{
    class LogPanel final : public IPanel
    {
    public:
        auto begin(EngineContext& ctx, EditorState& state) -> void override;
        auto draw(EngineContext& ctx, EditorState& state) -> void override;

    private:
        auto log_event(const LogEvent& event) -> void;

    private:
        Vector<LogEvent> m_log_entries = {};
        bool m_scroll_to_bottom = true;
    };
} // namespace Cobalt

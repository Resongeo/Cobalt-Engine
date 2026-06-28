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
        auto Begin(EditorState& state) -> void override;
        auto Draw(EditorState& state) -> void override;

    private:
        auto AddLogEvent(const LogEvent& event) -> void;

    private:
        Vector<LogEvent> _log_entries = {};
        bool _scroll_to_bottom = true;
    };
} // namespace Cobalt

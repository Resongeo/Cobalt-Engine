// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Platform/Window.hpp"

namespace Cobalt
{
    using DialogCallback = std::function<void(const Filepath& choice)>;

    struct DialogFileFilter
    {
        StringView name;
        StringView pattern;
    };

    class DialogManager final
    {
    public:
        auto init(const Window& window) -> void;
        auto show_save_dialog(const String& default_path, Span<DialogFileFilter> filters, DialogCallback&& on_complete) const -> void;

    private:
        SDL_Window* m_window_handle = nullptr;
    };
} // namespace Cobalt

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Platform/DialogManager.hpp"

#include <SDL3/SDL_dialog.h>

namespace Cobalt
{
    auto DialogManager::Init() -> void {
        _window_handle = Window::Get().GetHandle();
    }

    auto DialogManager::ShowSaveDialog(const String& default_path, Span<DialogFileFilter> filters, DialogCallback&& on_complete) const
            -> void {
        static auto sdl_filters = Vector<SDL_DialogFileFilter>{};

        auto* callback_ptr = new DialogCallback(std::move(on_complete));

        sdl_filters.clear();
        for (auto filter : filters) {
            sdl_filters.push_back(SDL_DialogFileFilter{filter.name.data(), filter.pattern.data()});
        }

        SDL_ShowSaveFileDialog(
                [](void* userdata, const char* const* filelist, int filter) {
                    const auto* cpp_callback = static_cast<DialogCallback*>(userdata);

                    if (filelist && *filelist) {
                        (*cpp_callback)(Filepath(*filelist));
                    } else {
                        (*cpp_callback)(Filepath());
                    }

                    delete cpp_callback;
                },
                callback_ptr, _window_handle, sdl_filters.data(), sdl_filters.size(), default_path.c_str());
    }
} // namespace Cobalt

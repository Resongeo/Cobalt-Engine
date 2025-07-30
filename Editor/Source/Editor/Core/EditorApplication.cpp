// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"

#include "Engine/Core/Logger.hpp"

namespace Cobalt::Editor
{
    auto EditorApplication::on_begin() -> void {
        Engine::Logger::trace("Editor", "called on_attach");
    }

    auto EditorApplication::on_update() -> void {
        Engine::Logger::trace("Editor", "called on_update");
    }

    auto EditorApplication::on_end() -> void {
        Engine::Logger::trace("Editor", "called on_end");
    }
}

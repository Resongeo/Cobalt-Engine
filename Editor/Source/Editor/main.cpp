// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Engine/Core/EntryPoint.hpp"

auto Cobalt::create_application(CommandLineArgs args) -> Box<Engine::Application> {
    return Memory::make_box<Editor::EditorApplication>(args.count, args.args);
}

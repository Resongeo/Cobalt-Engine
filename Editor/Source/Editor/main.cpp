// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"
#include "Engine/Core/EntryPoint.hpp"

auto Cobalt::create_application() -> Box<Application> {
    return Memory::make_box<EditorApplication>();
}

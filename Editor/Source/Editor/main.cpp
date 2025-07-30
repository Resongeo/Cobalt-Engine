// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Editor/Core/EditorApplication.hpp"

// TODO: Process arguments
auto main() -> int {
    auto app = Cobalt::Editor::EditorApplication();
    app.run();
}
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include <entt/entity/entity.hpp>

namespace Cobalt::Editor
{
    struct EditorState
    {
        entt::entity selected_entity = entt::null;
    };
}
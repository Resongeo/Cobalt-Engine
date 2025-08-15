// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Scene/Scene.hpp"

namespace Cobalt::Engine
{
    Scene::Scene(const String& name)
        : m_name(name) {}

    auto Scene::name() -> String& {
        return m_name;
    }

    auto Scene::registry() -> entt::registry& {
        return m_registry;
    }
}
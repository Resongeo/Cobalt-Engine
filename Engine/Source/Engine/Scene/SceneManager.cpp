// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Scene/SceneManager.hpp"

namespace Cobalt::Engine
{
    SceneManager::~SceneManager() {
        delete m_default_scene;
    }

    auto SceneManager::init() -> void {
        m_default_scene = new Scene("Default");
    }

    auto SceneManager::scenes() -> Vector<Scene> {
        return m_scenes;
    }

    auto SceneManager::active_scene() const -> Scene* {
        return m_active_scene;
    }

    auto SceneManager::default_scene() const -> Scene* {
        return m_default_scene;
    }
}

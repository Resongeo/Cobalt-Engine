// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Scene/SceneManager.hpp"

namespace Cobalt::Engine
{
    auto SceneManager::scenes() -> Vector<std::unique_ptr<Scene>>& {
        return m_scenes;
    }

    auto SceneManager::active_scene() const -> Scene* {
        return m_active_scene;
    }

    auto SceneManager::create_default_scene() -> void {
        const auto& default_scene = m_scenes.emplace_back(
            std::make_unique<Scene>("Default")
        );
        m_active_scene = default_scene.get();
    }
}

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Scene/Scene.hpp"

#include <memory>

namespace Cobalt::Engine
{
    class SceneManager final
    {
    public:
        ~SceneManager() = default;

        auto scenes() -> Vector<std::unique_ptr<Scene>>&;
        auto active_scene() const -> Scene*;
        auto create_default_scene() -> void;

    private:
        Vector<std::unique_ptr<Scene>> m_scenes;
        Scene* m_active_scene = nullptr;
    };
}
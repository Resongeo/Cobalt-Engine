// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Scene/Scene.hpp"

namespace Cobalt::Engine
{
    class SceneManager final
    {
    public:
        ~SceneManager();

        auto init() -> void;
        auto scenes() -> Vector<Scene>;
        auto active_scene() const -> Scene*;
        auto default_scene() const -> Scene*;

    private:
        Vector<Scene> m_scenes;
        Scene* m_active_scene = nullptr;
        Scene* m_default_scene = nullptr;
    };
}
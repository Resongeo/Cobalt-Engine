// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Math.hpp"
#include "Engine/Core/Types/Containers.hpp"

namespace Cobalt::Engine
{
    class Shader final
    {
    public:
        Shader() = default;

        auto create(const char* vertex_path, const char* fragment_path) -> void;

        auto bind() const -> void;
        auto unbind() -> void;

        auto set_mat4(const char* name, const Mat4& value) -> void;

    private:
        auto _uniform_location(const char* name) -> i32;

    private:
        HashMap<const char*, i32> m_uniform_locations = {};
        u32 m_renderer_id = 0;
    };
}
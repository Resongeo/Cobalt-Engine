// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#pragma once

#include "Engine/Core/Types/Containers.hpp"
#include "Engine/Core/Types/Math.hpp"

namespace Cobalt
{
    class Shader final
    {
    public:
        Shader() = default;

        auto bind() const -> void;
        auto unbind() const -> void;

        auto create_from_file(const char* vertex_path, const char* fragment_path) -> bool;
        auto create_fallback() -> bool;
        auto set_mat4(const char* name, const Mat4& value) -> void;
        auto set_int_array(const char* name, const i32* values, const u32 count) -> void;

    private:
        auto create(const String& vertex_source, const String& fragment_source) -> bool;
        auto uniform_location(const char* name) -> i32;

    private:
        HashMap<const char*, i32> m_uniform_locations = {};
        u32 m_renderer_id = 0;
    };
} // namespace Cobalt

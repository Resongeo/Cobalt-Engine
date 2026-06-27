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

        auto Bind() const -> void;
        auto Unbind() const -> void;

        auto CreateFromFile(const char* vertex_path, const char* fragment_path) -> bool;
        auto CreateFallback() -> bool;
        auto SetMat4(const char* name, const Mat4& value) -> void;
        auto SetIntArray(const char* name, const i32* values, u32 count) -> void;

    private:
        auto Create(const String& vertex_source, const String& fragment_source) -> bool;
        auto GetUniformLocation(const char* name) -> i32;

    private:
        HashMap<const char*, i32> _uniform_locations = {};
        u32 _renderer_id = 0;
    };
} // namespace Cobalt

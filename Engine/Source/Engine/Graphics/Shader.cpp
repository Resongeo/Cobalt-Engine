// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Shader.hpp"
#include "Engine/Core/File.hpp"
#include "Engine/Core/Log.hpp"

namespace Cobalt
{
    auto Shader::CreateFromFile(const char* vertex_path, const char* fragment_path) -> bool {
        if (!File::Exists(vertex_path) || !File::Exists(fragment_path)) {
            return false;
        }

        const auto vertex_source = File::Read(vertex_path);
        const auto fragment_source = File::Read(fragment_path);

        return Create(vertex_source, fragment_source);
    }

    auto Shader::CreateFallback() -> bool {
        const auto vertex_source = R"(
            #version 330 core
            layout(location = 0) in vec2 a_Position;
            layout(location = 1) in vec4 a_Color;
            uniform mat4 u_ViewProjection;
            out vec4 v_Color;
            void main() {
                gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
                v_Color = a_Color;
            }
        )";
        const auto fragment_source = R"(
            #version 330 core
            layout(location = 0) out vec4 FragColor;
            in vec4 v_Color;
            void main() {
                FragColor = v_Color;
            }
        )";

        return Create(vertex_source, fragment_source);
    }

    auto Shader::Bind() const -> void {
    }

    auto Shader::Unbind() const -> void {
    }

    auto Shader::SetMat4(const char* name, const Mat4& value) -> void {
    }

    auto Shader::SetIntArray(const char* name, const i32* values, const u32 count) -> void {
    }

    auto Shader::Create(const String& vertex_source, const String& fragment_source) -> bool {

        return true;
    }

    auto Shader::GetUniformLocation(const char* name) -> i32 {
        if (_uniform_locations.contains(name)) {
            return _uniform_locations[name];
        }

        return 0;
    }
} // namespace Cobalt

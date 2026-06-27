// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Somogyvári Benedek

#include "Engine/Graphics/Shader.hpp"
#include "Engine/Core/File.hpp"
#include "Engine/Core/Log.hpp"

#include <glad/gl.h>

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
        glUseProgram(_renderer_id);
    }

    auto Shader::Unbind() const -> void {
        glUseProgram(0);
    }

    auto Shader::SetMat4(const char* name, const Mat4& value) -> void {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    auto Shader::SetIntArray(const char* name, const i32* values, const u32 count) -> void {
        glUniform1iv(GetUniformLocation(name), count, values);
    }

    auto Shader::Create(const String& vertex_source, const String& fragment_source) -> bool {
        GLint is_compiled = 0;

        const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        const auto vertex_c_str = vertex_source.c_str();
        glShaderSource(vertex_shader, 1, &vertex_c_str, nullptr);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE) {
            GLint max_length = 0;
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

            Vector<GLchar> info_log(max_length);
            glGetShaderInfoLog(vertex_shader, max_length, &max_length, info_log.data());

            CORE_ERROR("Graphics::Shader: Vertex shader compilation error:\n{}", info_log.data());
            glDeleteShader(vertex_shader);

            return false;
        }

        const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        const auto fragment_c_str = fragment_source.c_str();
        glShaderSource(fragment_shader, 1, &fragment_c_str, nullptr);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE) {
            GLint max_length = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

            Vector<GLchar> info_log(max_length);
            glGetShaderInfoLog(fragment_shader, max_length, &max_length, info_log.data());

            CORE_ERROR("Graphics::Shader: Fragment shader compilation error:\n{}", info_log.data());
            glDeleteShader(fragment_shader);

            return false;
        }

        _renderer_id = glCreateProgram();
        glAttachShader(_renderer_id, vertex_shader);
        glAttachShader(_renderer_id, fragment_shader);
        glLinkProgram(_renderer_id);

        GLint is_linked = 0;
        glGetProgramiv(_renderer_id, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE) {
            GLint max_length = 0;
            glGetProgramiv(_renderer_id, GL_INFO_LOG_LENGTH, &max_length);

            Vector<GLchar> info_log(max_length);
            glGetProgramInfoLog(_renderer_id, max_length, &max_length, info_log.data());

            CORE_ERROR("Graphics::Shader: Shader linking error:\n{}", info_log.data());
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            glDeleteProgram(_renderer_id);
            _renderer_id = 0;

            return false;
        }

        glDetachShader(_renderer_id, vertex_shader);
        glDetachShader(_renderer_id, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        CORE_INFO("Graphics::Shader: Creating. ID: {}", _renderer_id);

        return true;
    }

    auto Shader::GetUniformLocation(const char* name) -> i32 {
        if (_uniform_locations.contains(name)) {
            return _uniform_locations[name];
        }

        const auto location = glGetUniformLocation(_renderer_id, name);
        _uniform_locations[name] = location;

        return location;
    }
} // namespace Cobalt

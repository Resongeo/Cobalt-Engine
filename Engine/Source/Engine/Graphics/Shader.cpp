// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Somogyvári Benedek

#include "Engine/Graphics/Shader.hpp"
#include "Engine/Core/File.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/gl.h>

namespace Cobalt::Engine
{
    auto Shader::create(const char* vertex_path, const char* fragment_path) -> void {
        const auto vertex_source = File::read(vertex_path);
        const auto fragment_source = File::read(fragment_path);

        GLint is_compiled = 0;

        const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        const auto vertex_c_string = vertex_source.c_str();
        glShaderSource(vertex_shader, 1, &vertex_c_string, nullptr);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE)
        {
            GLint max_length = 0;
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

            Vector<GLchar> info_log(max_length);
            glGetShaderInfoLog(vertex_shader, max_length, &max_length, info_log.data());

            Logger::error("shader", "Vertex shader compilation error:\n{}", info_log.data());
            glDeleteShader(vertex_shader);
        }

        const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        const auto fragment_c_string = fragment_source.c_str();
        glShaderSource(fragment_shader, 1, &fragment_c_string, nullptr);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
        if (is_compiled == GL_FALSE)
        {
            GLint max_length = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

            Vector<GLchar> info_log(max_length);
            glGetShaderInfoLog(fragment_shader, max_length, &max_length, info_log.data());

            Logger::error("shader", "Fragment shader compilation error:\n{}", info_log.data());
            glDeleteShader(fragment_shader);
        }

        m_renderer_id = glCreateProgram();
        glAttachShader(m_renderer_id, vertex_shader);
        glAttachShader(m_renderer_id, fragment_shader);
        glLinkProgram(m_renderer_id);

        GLint is_linked = 0;
        glGetProgramiv(m_renderer_id, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE)
        {
            GLint max_length = 0;
            glGetProgramiv(m_renderer_id, GL_INFO_LOG_LENGTH, &max_length);

            Vector<GLchar> info_log(max_length);
            glGetProgramInfoLog(m_renderer_id, max_length, &max_length, info_log.data());

            Logger::error("shader", "Shader linking error:\n{}", info_log.data());
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            glDeleteProgram(m_renderer_id);
            m_renderer_id = 0;
        }

        glDetachShader(m_renderer_id, vertex_shader);
        glDetachShader(m_renderer_id, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        Logger::trace("shader", "Created ID: {}", m_renderer_id);
    }

    auto Shader::bind() const -> void {
        glUseProgram(m_renderer_id);
    }

    auto Shader::unbind() -> void {
        glUseProgram(0);
    }

    auto Shader::set_mat4(const char* name, const Mat4& value) -> void {
        glUniformMatrix4fv(_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    auto Shader::_uniform_location(const char* name) -> i32 {
        if (m_uniform_locations.contains(name)) {
            return m_uniform_locations[name];
        }

        const auto location = glGetUniformLocation(m_renderer_id, name);
        m_uniform_locations[name] = location;

        return location;
    }
}

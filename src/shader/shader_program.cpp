/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include <epoxy/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "glare/shader/shader_program.hpp"
#include "glare/shader/shader.hpp"

namespace glare {

ShaderProgram::ShaderProgram()
{
    if ((m_id = glCreateProgram()) == 0) {
        throw std::runtime_error(
            "Couldn't create shader program: glCreateProgram() error"
        );
    }
}

ShaderProgram::ShaderProgram(std::initializer_list<Shader*> shaders)
  : ShaderProgram()
{
    for (auto shader : shaders) {
        add_shader(*shader);
    }
}

ShaderProgram::~ShaderProgram() noexcept
{
    glDeleteProgram(m_id);
}

void ShaderProgram::add_shader(Shader& shader)
{
    glAttachShader(m_id, shader.id());
}

void ShaderProgram::link()
{
    glLinkProgram(m_id);

    GLint status;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);

    if (status != GL_TRUE) {
        char msg[512];
        glGetProgramInfoLog(m_id, sizeof msg, nullptr, msg);

        throw std::runtime_error(
            "Couldn't link shader program: " + std::string(msg)
        );
    }
}

void ShaderProgram::use()
{
    glUseProgram(m_id);
}

ShaderProgram::Binary ShaderProgram::download_binary() const
{
    Binary binary;
    binary.data.resize(get_parameter(GL_PROGRAM_BINARY_LENGTH));

    glGetProgramBinary(
        m_id,
        binary.data.size(),
        nullptr,
        &binary.format,
        binary.data.data()
    );

    return binary;
}

void ShaderProgram::upload_binary(const ShaderProgram::Binary& bin)
{
    glProgramBinary(m_id, bin.format, bin.data.data(), bin.data.size());
}

GLint ShaderProgram::uniform_location(const std::string& name)
{
    GLint location = glGetUniformLocation(m_id, name.c_str());

    if (location == -1) {
        /*std::cerr << "Warning: Couldn't find uniform location '"
                  << name << "'" << std::endl;*/
    }

    return location;
}

GLint ShaderProgram::get_parameter(GLenum param) const
{
    GLint value;

    glGetProgramiv(m_id, param, &value);
    return value;
}

void ShaderProgram::set_parameter(GLenum param, int value)
{
    glProgramParameteri(m_id, param, value);
}

void ShaderProgram::set_uniform(GLint loc, float n)
{ glUniform1f(loc, n); }

void ShaderProgram::set_uniform(GLint loc, const glm::vec2& vec)
{ glUniform2f(loc, vec.x, vec.y); }

void ShaderProgram::set_uniform(GLint loc, const glm::vec3& vec)
{ glUniform3f(loc, vec.x, vec.y, vec.z); }

void ShaderProgram::set_uniform(GLint loc, const glm::vec4& vec)
{ glUniform4f(loc, vec.x, vec.y, vec.z, vec.w); }

void ShaderProgram::set_uniform(GLint loc, int n)
{ glUniform1i(loc, n); }

void ShaderProgram::set_uniform(GLint loc, const glm::ivec2& vec)
{ glUniform2i(loc, vec.x, vec.y); }

void ShaderProgram::set_uniform(GLint loc, const glm::ivec3& vec)
{ glUniform3i(loc, vec.x, vec.y, vec.z); }

void ShaderProgram::set_uniform(GLint loc, const glm::ivec4& vec)
{ glUniform4i(loc, vec.x, vec.y, vec.z, vec.w); }

void ShaderProgram::set_uniform(GLint loc, unsigned n)
{ glUniform1ui(loc, n); }

void ShaderProgram::set_uniform(GLint loc, const glm::uvec2& vec)
{ glUniform2ui(loc, vec.x, vec.y); }

void ShaderProgram::set_uniform(GLint loc, const glm::uvec3& vec)
{ glUniform3ui(loc, vec.x, vec.y, vec.z); }

void ShaderProgram::set_uniform(GLint loc, const glm::uvec4& vec)
{ glUniform4ui(loc, vec.x, vec.y, vec.z, vec.w); }

void ShaderProgram::set_uniform(GLint loc, const glm::mat4& mat)
{ glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat)); }

void ShaderProgram::set_uniform(GLint loc, const glm::mat3& mat)
{ glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat)); }

void ShaderProgram::set_uniform(GLint loc, const glm::mat2& mat)
{ glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(mat)); }

} // ns glare

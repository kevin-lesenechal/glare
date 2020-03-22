/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include <initializer_list>
#include <string>
#include <vector>

namespace glare {

class Shader;

class ShaderProgram
{
public:
    struct Binary {
        GLenum format;
        std::vector<uint8_t> data;
    };

    ShaderProgram();

    explicit ShaderProgram(std::initializer_list<Shader*> shaders);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ~ShaderProgram() noexcept;

    void add_shader(Shader& shader);
    void link();
    void use();

    Binary download_binary() const;
    void upload_binary(const Binary& bin);

    GLint uniform_location(const std::string& name);

    GLint get_parameter(GLenum param) const;
    void set_parameter(GLenum param, int value);

    void set_uniform(GLint loc, float n);
    void set_uniform(GLint loc, const glm::vec2& vec);
    void set_uniform(GLint loc, const glm::vec3& vec);
    void set_uniform(GLint loc, const glm::vec4& vec);

    void set_uniform(GLint loc, int n);
    void set_uniform(GLint loc, const glm::ivec2& vec);
    void set_uniform(GLint loc, const glm::ivec3& vec);
    void set_uniform(GLint loc, const glm::ivec4& vec);

    void set_uniform(GLint loc, unsigned n);
    void set_uniform(GLint loc, const glm::uvec2& vec);
    void set_uniform(GLint loc, const glm::uvec3& vec);
    void set_uniform(GLint loc, const glm::uvec4& vec);

    void set_uniform(GLint loc, const glm::mat4& mat);
    void set_uniform(GLint loc, const glm::mat3& mat);
    void set_uniform(GLint loc, const glm::mat2& mat);

    template<typename T>
    void set_uniform(const std::string& loc_name, T&& value)
    {
        set_uniform(uniform_location(loc_name), std::forward<T>(value));
    }

private:
    GLuint m_id;
};

} // ns glare

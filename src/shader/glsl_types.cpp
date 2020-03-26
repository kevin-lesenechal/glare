/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/shader/glsl_types.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sstream>
#include <iomanip>

namespace glare {

std::string glsl_literal_value(bool value)
{
    return value ? "true" : "false";
}

std::string glsl_literal_value(int value)
{
    return std::to_string(value);
}

std::string glsl_literal_value(unsigned value)
{
    return std::to_string(value);
}

std::string glsl_literal_value(float value)
{
    std::stringstream ss;
    ss << std::setprecision(7) << value;
    std::string str = ss.str();

    if (str.find('.') == std::string::npos) {
        str += ".0";
    }

    return str + "f";
}

std::string glsl_literal_value(double value)
{
    std::stringstream ss;
    ss << std::setprecision(17) << value;
    std::string str = ss.str();

    if (str.find('.') == std::string::npos) {
        str += ".0";
    }

    return str;
}

std::string glsl_literal_value(const glm::vec2& vec)
{
    return "vec2("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ")";
}

std::string glsl_literal_value(const glm::vec3& vec)
{
    return "vec3("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ")";
}

std::string glsl_literal_value(const glm::vec4& vec)
{
    return "vec4("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ", "
           + glsl_literal_value(vec.w) + ")";
}

std::string glsl_literal_value(const glm::ivec2& vec)
{
    return "ivec2("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ")";
}

std::string glsl_literal_value(const glm::ivec3& vec)
{
    return "ivec3("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ")";
}

std::string glsl_literal_value(const glm::ivec4& vec)
{
    return "ivec4("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ", "
           + glsl_literal_value(vec.w) + ")";
}

std::string glsl_literal_value(const glm::uvec2& vec)
{
    return "uvec2("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ")";
}

std::string glsl_literal_value(const glm::uvec3& vec)
{
    return "uvec3("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ")";
}

std::string glsl_literal_value(const glm::uvec4& vec)
{
    return "uvec4("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ", "
           + glsl_literal_value(vec.w) + ")";
}

std::string glsl_literal_value(const glm::dvec2& vec)
{
    return "dvec2("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ")";
}

std::string glsl_literal_value(const glm::dvec3& vec)
{
    return "dvec3("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ")";
}

std::string glsl_literal_value(const glm::dvec4& vec)
{
    return "dvec4("
           + glsl_literal_value(vec.x) + ", "
           + glsl_literal_value(vec.y) + ", "
           + glsl_literal_value(vec.z) + ", "
           + glsl_literal_value(vec.w) + ")";
}

} // ns glare

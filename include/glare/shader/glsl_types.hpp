/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <string>
#include <glm/fwd.hpp>

namespace glare {

std::string glsl_literal_value(bool value);
std::string glsl_literal_value(int value);
std::string glsl_literal_value(unsigned value);
std::string glsl_literal_value(float value);
std::string glsl_literal_value(double value);
std::string glsl_literal_value(const glm::vec2& vec);
std::string glsl_literal_value(const glm::vec3& vec);
std::string glsl_literal_value(const glm::vec4& vec);
std::string glsl_literal_value(const glm::ivec2& vec);
std::string glsl_literal_value(const glm::ivec3& vec);
std::string glsl_literal_value(const glm::ivec4& vec);
std::string glsl_literal_value(const glm::uvec2& vec);
std::string glsl_literal_value(const glm::uvec3& vec);
std::string glsl_literal_value(const glm::uvec4& vec);
std::string glsl_literal_value(const glm::dvec2& vec);
std::string glsl_literal_value(const glm::dvec3& vec);
std::string glsl_literal_value(const glm::dvec4& vec);

} // ns glare

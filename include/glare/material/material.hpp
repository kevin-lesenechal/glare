/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <glm/vec3.hpp>
#include <memory>

namespace glare {

class Texture;
class ShaderProgram;

struct Material
{
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Texture> specular_map;
    std::shared_ptr<Texture> normal_map;
    glm::vec3 ambient_color = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuse_color = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular_color = glm::vec3(1.0f, 1.0f, 1.0f);
    float     shininess = 32.0f;

    Material() = default;

    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;
    Material(Material&&) noexcept = default;
    Material& operator=(Material&&) = default;
};

} // ns glare

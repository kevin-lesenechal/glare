/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <string>
#include <glm/vec3.hpp>

namespace glare {

class LightConfig
{
public:
    LightConfig(
        const std::string& title,
        const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3& ambient = glm::vec3(0.1f, 0.1f, 0.1f),
        const glm::vec3& diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
        const glm::vec3& specular = glm::vec3(1.0f, 1.0f, 1.0f)
    );

    void draw();

public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

private:
    void update_colors();

private:
    std::string m_title;
    float       m_color_temp;
    glm::vec3   m_color;
    float       m_intensity = 0.5f;
    float       m_ambient_factor = 0.05f;
    float       m_diffuse_factor = 0.5f;
    float       m_spec_factor = 0.9f;
};

} // ns glare

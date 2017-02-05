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
    std::string m_title;
};

} // ns glare

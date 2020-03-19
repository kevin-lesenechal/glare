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
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float     shininess;

    Material() = default;

    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;
    Material(Material&&) noexcept = default;
    Material& operator=(Material&&) = default;
};

} // ns glare

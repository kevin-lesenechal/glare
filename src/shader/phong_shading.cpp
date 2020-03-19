#include "glare/shader/phong_shading.hpp"
#include "glare/shader/shader_program.hpp"
#include "glare/material/material.hpp"
#include "glare/material/texture.hpp"

namespace glare {

PhongShading::PhongShading(ShaderProgram& program,
                           const std::string& material_var)
  : m_program(program),
    m_material_var(material_var)
{}

void PhongShading::set_material(const Material& material)
{
    if (material.texture) {
        m_program.set_uniform(m_material_var + ".texture",
                              material.texture->unit());
        material.texture->bind();
    }
    if (material.specular_map) {
        m_program.set_uniform(m_material_var + ".specular_map",
                              material.specular_map->unit());
        material.specular_map->bind();
    }
    if (material.normal_map) {
        m_program.set_uniform(m_material_var + ".normal_map",
                              material.normal_map->unit());
        material.normal_map->bind();
    }

    m_program.set_uniform(m_material_var + ".ambient_color",
                          material.ambient_color);
    m_program.set_uniform(m_material_var + ".diffuse_color",
                          material.diffuse_color);
    m_program.set_uniform(m_material_var + ".specular_color",
                          material.specular_color);
    m_program.set_uniform(m_material_var + ".shininess",
                          material.shininess);

    m_program.set_uniform(m_material_var + ".has_specular_map",
                          static_cast<bool>(material.specular_map));
    m_program.set_uniform(m_material_var + ".has_normal_map",
                          static_cast<bool>(material.normal_map));
}

} // ns glare

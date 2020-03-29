/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

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
                              static_cast<int>(m_texture_unit));
        material.texture->bind(m_texture_unit);
    }
    if (material.specular_map) {
        m_program.set_uniform(m_material_var + ".specular_map",
                              static_cast<int>(m_specular_map_unit));
        material.specular_map->bind(m_specular_map_unit);
    }
    if (material.normal_map) {
        m_program.set_uniform(m_material_var + ".normal_map",
                              static_cast<int>(m_normal_map_unit));
        material.normal_map->bind(m_normal_map_unit);
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

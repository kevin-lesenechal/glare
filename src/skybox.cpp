/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/skybox.hpp"
#include "glare/material/texture.hpp"

namespace glare {

Skybox::Skybox(std::shared_ptr<Texture> cubemap,
               ShaderProgram& skybox_program)
  : m_unit_cube(std::shared_ptr<Material>(new Material{
      std::move(cubemap)
    })),
    m_program(skybox_program),
    m_shading(m_program)
{}

void Skybox::draw(const glm::mat4& projection_mx, const glm::mat4& view_mx)
{
    m_program.use();

    glm::mat4 center_view = glm::mat4(glm::mat3(view_mx));
    m_program.set_uniform("g_view_mx", center_view);
    m_program.set_uniform("g_proj_mx", projection_mx);

    glFrontFace(GL_CW);
    glDepthFunc(GL_LEQUAL);
    m_unit_cube.draw(m_shading);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CCW);
}

Skybox::SkyboxShading::SkyboxShading(ShaderProgram& program)
  : m_program(program)
{}

void Skybox::SkyboxShading::set_material(const Material& material)
{
    material.texture->bind(0);
    m_program.set_uniform("g_cubemap", 0);
}

} // ns glare

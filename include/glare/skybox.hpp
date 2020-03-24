/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/primitive/unit_cube.hpp"
#include "glare/shader/shader_program.hpp"
#include "glare/shader/shading_interface.hpp"

namespace glare {

class Texture;

class Skybox
{
private:
    class SkyboxShading : public ShadingInterface
    {
    public:
        SkyboxShading(ShaderProgram& program);
        ShaderProgram& m_program;
        void set_material(const Material& material) override;
    };

public:
    Skybox(std::shared_ptr<Texture> cubemap,
           ShaderProgram& skybox_program);

    void draw(const glm::mat4& projection_mx, const glm::mat4& view_mx);

private:
    UnitCube m_unit_cube;
    ShaderProgram& m_program;
    SkyboxShading m_shading;
};

} // ns glare

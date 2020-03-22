/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "shading_interface.hpp"

#include <string>

namespace glare {

struct Material;
class ShaderProgram;

class PhongShading : public ShadingInterface
{
public:
    explicit PhongShading(ShaderProgram& program,
                          const std::string& material_var = "g_material");

    void set_material(const Material& material) override;

private:
    ShaderProgram&  m_program;
    std::string     m_material_var;
};

} // ns glare

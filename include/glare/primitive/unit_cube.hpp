/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/model/mesh.hpp"

namespace glare {

class ShadingInterface;
class Material;

class UnitCube
{
public:
    explicit UnitCube(std::shared_ptr<Material> material);

    void draw();
    void draw(ShadingInterface& shading);

private:
    Mesh m_mesh;
};

} // ns glare

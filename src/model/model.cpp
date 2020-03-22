/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/model/model.hpp"

namespace glare {

void Model::draw()
{
    for (auto& mesh : m_meshes) {
        mesh.draw();
    }
}

void Model::draw(ShadingInterface& shading)
{
    for (auto& mesh : m_meshes) {
        mesh.draw(shading);
    }
}

void Model::push_mesh(Mesh&& mesh)
{
    m_meshes.push_back(std::move(mesh));
}

} // ns glare

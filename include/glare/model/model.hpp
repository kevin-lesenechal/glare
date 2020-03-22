/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "mesh.hpp"

#include <vector>

namespace glare {

class ShadingInterface;

class Model
{
public:
    Model() = default;

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&&) = default;
    Model& operator=(Model&&) = default;

    void draw(ShadingInterface& shading);

    void push_mesh(Mesh&& mesh);

private:
    std::vector<Mesh> m_meshes;
};

} // ns glare

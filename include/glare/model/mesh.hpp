/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/opengl/vertex_array.hpp"
#include "glare/opengl/buffer.hpp"
#include "glare/material/material.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>

namespace glare {

class ShadingInterface;

class Mesh
{
public:
    struct VertexAttr {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coord;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

public:
    Mesh(
        const std::vector<VertexAttr>& vertices,
        const std::vector<unsigned>& indices,
        std::shared_ptr<Material> material
    );

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    void draw();
    void draw(ShadingInterface& shading);

private:
    VertexArray m_vao;
    VertexBuffer<float> m_vertices;
    ElementArrayBuffer<unsigned> m_indices;
    size_t m_indices_count;
    std::shared_ptr<Material> m_material;
};

} // ns glare

#include "glare/model/mesh.hpp"

#include <cstddef>

namespace glare {

Mesh::Mesh(const std::vector<VertexAttr>& vertices,
           const std::vector<unsigned>& indices)
  : m_indices_count(indices.size())
{
    unsigned values_per_vert = 3 + 3 + 2 + 3 + 3;
    size_t vert_memsz = sizeof (VertexAttr);

    m_vertices.upload(
        BufferUsage::StaticDraw,
        &vertices[0].position.x,
        vertices.size() * values_per_vert
    );

    m_vao.attach(0, m_vertices, 3, vert_memsz, offsetof(VertexAttr, position));
    m_vao.attach(1, m_vertices, 3, vert_memsz, offsetof(VertexAttr, normal));
    m_vao.attach(2, m_vertices, 2, vert_memsz, offsetof(VertexAttr, tex_coord));
    m_vao.attach(3, m_vertices, 3, vert_memsz, offsetof(VertexAttr, tangent));
    m_vao.attach(4, m_vertices, 3, vert_memsz, offsetof(VertexAttr, bitangent));

    m_indices.upload(BufferUsage::StaticDraw, indices);
}

void Mesh::draw(ShaderProgram& program)
{
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_INT, nullptr);
}

} // ns glare

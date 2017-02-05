#pragma once

#include "glare/opengl/vertex_array.hpp"
#include "glare/opengl/buffer.hpp"
#include "glare/opengl/shader_program.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>

namespace glare {

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
        const std::vector<unsigned>& indices
    );

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    void draw(ShaderProgram& program);

private:
    VertexArray m_vao;
    VertexBuffer<float> m_vertices;
    ElementArrayBuffer<unsigned> m_indices;
    size_t m_indices_count;
};

} // ns glare

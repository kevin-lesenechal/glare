#pragma once

#include "mesh.hpp"
#include "glare/opengl/shader_program.hpp"

#include <vector>

namespace glare {

class Model
{
public:
    Model() = default;

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
    Model(Model&&) = default;
    Model& operator=(Model&&) = default;

    void draw(ShaderProgram& program);

    void push_mesh(Mesh&& mesh);

private:
    std::vector<Mesh> m_meshes;
};

} // ns glare

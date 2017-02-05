#include "glare/model/model.hpp"

namespace glare {

void Model::draw(ShaderProgram& program)
{
    for (auto& mesh : m_meshes) {
        mesh.draw(program);
    }
}

void Model::push_mesh(Mesh&& mesh)
{
    m_meshes.push_back(std::move(mesh));
}

} // ns glare

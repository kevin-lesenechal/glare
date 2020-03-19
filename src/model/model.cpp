#include "glare/model/model.hpp"

namespace glare {

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

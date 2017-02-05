#pragma once

#include "model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace glare {

class ModelLoader
{
public:
    explicit ModelLoader(Assimp::Importer& importer);

    [[nodiscard]] Model load_from_file(const std::string& file_path);

private:
    void process_node(Model& model, const aiScene* scene, const aiNode* node);
    [[nodiscard]] Mesh load_mesh(const aiScene* scene, const aiMesh* mesh);

private:
    Assimp::Importer& m_importer;
};

} // ns glare

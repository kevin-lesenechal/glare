#include "glare/model/model_loader.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace glare {

ModelLoader::ModelLoader(Assimp::Importer& importer)
  : m_importer(importer)
{}

Model ModelLoader::load_from_file(const std::string& file_path)
{
    const aiScene* scene = m_importer.ReadFile(
        file_path,
        aiProcess_Triangulate
        | aiProcess_GenNormals | aiProcess_CalcTangentSpace
        | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph
    );

    if (scene == nullptr
        || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
        || scene->mRootNode == nullptr) {
        throw 42; // FIXME: proper exception
    }

    Model model;

    process_node(model, scene, scene->mRootNode);

    return model;
}

void ModelLoader::process_node(Model& model,
                               const aiScene* scene,
                               const aiNode* node)
{
    for (unsigned i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model.push_mesh(load_mesh(scene, mesh));
    }

    for (unsigned i = 0; i < node->mNumChildren; ++i) {
        process_node(model, scene, node->mChildren[i]);
    }
}

Mesh ModelLoader::load_mesh(const aiScene* scene, const aiMesh* mesh)
{
    std::vector<Mesh::VertexAttr> vertices;
    vertices.reserve(mesh->mNumVertices);

    for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
        glm::vec2 tex_coord(0.0f, 0.0f);
        if (mesh->mTextureCoords[0]) {
            tex_coord = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }

        vertices.push_back({
            {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z},
            {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z},
            tex_coord,
            {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z},
            {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z}
        });
    }

    std::vector<unsigned> indices;
    indices.reserve(mesh->mNumFaces * 3);

    for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
        for (unsigned j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    return Mesh(vertices, indices);
}

} // ns glare

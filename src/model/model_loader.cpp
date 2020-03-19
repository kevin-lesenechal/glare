#include "glare/model/model_loader.hpp"
#include "glare/material/texture_library_interface.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace glare {

ModelLoader::ModelLoader(Assimp::Importer& importer,
                         TextureLibraryInterface& texture_library)
  : m_importer(importer),
    m_texture_library(texture_library)
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

    auto material = std::make_shared<Material>();

    aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
    aiColor3D color;

    ai_material->Get(AI_MATKEY_SHININESS, &material->shininess, nullptr);
    ai_material->Get(AI_MATKEY_COLOR_AMBIENT, &color, nullptr);
    material->ambient_color = {color.r, color.g, color.b};
    ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, &color, nullptr);
    material->diffuse_color = {color.r, color.g, color.b};
    ai_material->Get(AI_MATKEY_COLOR_SPECULAR, &color, nullptr);
    material->specular_color = {color.r, color.g, color.b};

    load_mesh_texture(ai_material, aiTextureType_DIFFUSE, *material);
    load_mesh_texture(ai_material, aiTextureType_SPECULAR, *material);
    load_mesh_texture(ai_material, aiTextureType_NORMALS, *material);
    load_mesh_texture(ai_material, aiTextureType_HEIGHT, *material);

    return Mesh(vertices, indices, std::move(material));
}

void ModelLoader::load_mesh_texture(aiMaterial* ai_material,
                                    aiTextureType texture_type,
                                    Material& material)
{
    unsigned tex_count = ai_material->GetTextureCount(texture_type);
    if (tex_count == 0) {
        return;
    }

    int unit;
    switch (texture_type) {
        case aiTextureType_DIFFUSE:     unit = 0; break;
        case aiTextureType_SPECULAR:    unit = 1; break;
        case aiTextureType_NORMALS:
        case aiTextureType_HEIGHT:      unit = 2; break;
        default:
            return;
    }

    aiString ai_name;
    // TODO: handle multiple textures
    ai_material->GetTexture(texture_type, 0, &ai_name);
    std::string name(ai_name.C_Str());
    std::shared_ptr<Texture> texture = m_texture_library.get_texture(name, unit);

    switch (texture_type) {
        case aiTextureType_DIFFUSE:
            material.texture = std::move(texture);
            break;

        case aiTextureType_SPECULAR:
            material.specular_map = std::move(texture);
            break;

        case aiTextureType_NORMALS:
        case aiTextureType_HEIGHT:
            material.normal_map = std::move(texture);
            break;

        default:
            std::abort();
    }
}

} // ns glare

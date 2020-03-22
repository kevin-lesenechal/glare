/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace glare {

class TextureLibraryInterface;

class ModelLoader
{
public:
    ModelLoader(Assimp::Importer& importer,
                TextureLibraryInterface& texture_library);

    [[nodiscard]]
    Model load_from_file(const std::string& file_path);

private:
    void process_node(Model& model, const aiScene* scene, const aiNode* node);

    [[nodiscard]]
    Mesh load_mesh(const aiScene* scene, const aiMesh* mesh);

    void load_mesh_texture(aiMaterial* ai_material,
                           aiTextureType texture_type,
                           Material& material);

private:
    Assimp::Importer& m_importer;
    TextureLibraryInterface& m_texture_library;
};

} // ns glare

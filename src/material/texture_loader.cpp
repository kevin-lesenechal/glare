/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/material/texture_loader.hpp"
#include "glare/material/texture.hpp"
#include "glare/material/ktx_loader.hpp"
#include "glare/opengl/extensions.hpp"

#include <epoxy/gl.h>
#include <stb_image.h>
#include <fstream>

namespace fs = std::filesystem;

namespace glare {

Texture TextureLoader::load_from_file(const fs::path& file_path, int unit)
{
    if (file_path.extension() == ".ktx") {
        return load_from_ktx_file(file_path, unit);
    }

    Texture texture(Texture::Type::Texture2D, unit);
    load_into(file_path, texture);
    texture.generate_mipmap();
    texture.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    texture.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (ext::has_anisotropic_filter) {
        texture.set_parameter(
            GL_TEXTURE_MAX_ANISOTROPY,
            static_cast<float>(GL_MAX_TEXTURE_MAX_ANISOTROPY)
        );
    }

    return texture;
}

Texture TextureLoader::load_from_ktx_file(const fs::path& file_path, int unit)
{
    KtxFileLoader loader;

    std::ifstream stream;
    stream.exceptions(std::ifstream::failbit);
    stream.open(file_path, std::ios::binary);

    return loader.load_texture(stream, unit);
}

void TextureLoader::load_into(const std::filesystem::path& file_path,
                              Texture& texture)
{
    load_into(file_path, texture, static_cast<GLenum>(texture.type()));
}

void TextureLoader::load_into(const std::filesystem::path& file_path,
                              Texture& texture,
                              GLenum target)
{
    int tex_width, tex_height, tex_channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_data = stbi_load(
        file_path.c_str(),
        &tex_width, &tex_height,
        &tex_channels,
        4
    );

    if (tex_data == nullptr) {
        throw std::runtime_error(
            "Couldn't load texture image '" + file_path.string()
            + "': stbi_load() error"
        );
    }

    // FIXME: set proper source format (RGB / RGBA)
    texture.set_image(
        target,
        0,
        Size2D(tex_width, tex_height),
        GL_RGBA8,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex_data
    );

    stbi_image_free(tex_data);
}

} // ns glare

/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/material/texture_loader.hpp"
#include "glare/material/texture.hpp"
#include "glare/material/ktx_loader.hpp"

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

    int tex_width, tex_height, tex_channels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_data = stbi_load(
        file_path.c_str(),
        &tex_width, &tex_height,
        &tex_channels,
        4
    );

    if (tex_data == nullptr) {
        throw 42; // FIXME: proper exception
    }

    Texture texture(Texture::Type::Texture2D, unit);
    // FIXME: set proper source format (RGB / RGBA)
    texture.set_image(0, tex_width, tex_height, GL_RGB,
                      Texture::PixelFormat::RGBA, GL_UNSIGNED_BYTE, tex_data);
    texture.generate_mipmap();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (epoxy_has_gl_extension("GL_ARB_texture_filter_anisotropic")) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY,
                        GL_MAX_TEXTURE_MAX_ANISOTROPY);
    }

    stbi_image_free(tex_data);

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

} // ns glare

/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <epoxy/gl.h>
#include <filesystem>

namespace glare {

class Texture;

class TextureLoader
{
public:
    [[nodiscard]]
    Texture load_from_file(const std::filesystem::path& file_path,
                           int unit = 0);

    void load_into(const std::filesystem::path& file_path, Texture& texture);

    void load_into(const std::filesystem::path& file_path,
                   Texture& texture,
                   GLenum target);

private:
    [[nodiscard]]
    Texture load_from_ktx_file(const std::filesystem::path& file_path,
                               int unit = 0);
};

} // ns glare

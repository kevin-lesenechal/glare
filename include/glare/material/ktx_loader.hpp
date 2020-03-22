/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <istream>
#include <simple_ktx.hpp>

#include "glare/material/texture.hpp"

namespace glare {

class KtxFileLoader
{
public:
    KtxFileLoader();

    Texture load_texture(std::istream& input, int unit = 0);

private:
    Texture::Type texture_type(const simple_ktx::KtxFileHeader& ktx_header);
};

} // ns glare

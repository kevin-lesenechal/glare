/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <string>
#include <memory>

namespace glare {

class Texture;

class TextureLibraryInterface
{
public:
    virtual ~TextureLibraryInterface() noexcept = default;

    virtual std::shared_ptr<Texture> get_texture(const std::string& name,
                                                 int unit) = 0;
};

} // ns glare

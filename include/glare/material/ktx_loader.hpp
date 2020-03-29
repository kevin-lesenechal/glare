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
#include "glare/null_logger.hpp"

namespace glare {

class KtxFileLoader
{
public:
    explicit KtxFileLoader(LoggerInterface& logger = null_logger);

    Texture load_texture(std::istream& input);

private:
    Texture::Type texture_type(const simple_ktx::KtxFileHeader& ktx_header);

private:
    LoggerInterface& m_logger;
};

} // ns glare

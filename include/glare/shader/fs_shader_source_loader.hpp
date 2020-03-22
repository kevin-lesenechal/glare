/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "shader_source_loader_interface.hpp"

namespace glare {

class FsShaderSourceLoader : public ShaderSourceLoaderInterface
{
public:
    virtual std::string load_source(const std::string& location) override;
};

} // ns glare

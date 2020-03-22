/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <string>

namespace glare {

class ShaderSourceLoaderInterface
{
public:
    virtual ~ShaderSourceLoaderInterface() = default;

    virtual std::string load_source(const std::string& location) = 0;
};

} // ns glare

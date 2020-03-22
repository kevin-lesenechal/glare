/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

namespace glare {

struct Material;

class ShadingInterface
{
public:
    virtual ~ShadingInterface() = default;

    virtual void set_material(const Material& material) = 0;
};

}

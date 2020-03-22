/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/ui/checkbox.hpp"

namespace glare {

class DebugWindow
{
public:
    DebugWindow();

    void draw();

private:
    double          m_prev_time;
    glare::Checkbox m_wireframe;
    glare::Checkbox m_vsync;
    glare::Checkbox m_culling;
};

} // ns glare

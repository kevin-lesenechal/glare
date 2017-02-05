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

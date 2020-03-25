/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <GLFW/glfw3.h>

namespace glare {

struct MouseButtonEvent
{
    enum class Action : bool {
        Down = false,
        Up   = true
    };

    enum class Button
    {
        Left    = GLFW_MOUSE_BUTTON_LEFT,
        Right   = GLFW_MOUSE_BUTTON_RIGHT,
        Middle  = GLFW_MOUSE_BUTTON_MIDDLE,
        Button4 = GLFW_MOUSE_BUTTON_4,
        Button5 = GLFW_MOUSE_BUTTON_5,
        Button6 = GLFW_MOUSE_BUTTON_6,
        Button7 = GLFW_MOUSE_BUTTON_7,
        Button8 = GLFW_MOUSE_BUTTON_8
    };

    Action action;
    Button button;
};

} // ns glare

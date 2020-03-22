/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <functional>

namespace glare {

class Checkbox
{
public:
    Checkbox(const char* label, bool checked, std::function<void(bool)> callback);

    void draw();

private:
    const char* m_label;
    bool m_checked;
    std::function<void(bool)> m_callback;
};

} // ns glare

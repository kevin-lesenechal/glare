/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

namespace glare::ui {

bool color_temp_picker(const char* label, float* value,
                       float min = 500.0f, float max = 12000.0f);

} // ns glare

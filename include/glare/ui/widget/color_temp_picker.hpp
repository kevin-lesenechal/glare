#pragma once

namespace glare::ui {

bool color_temp_picker(const char* label, float* value,
                       float min = 500.0f, float max = 12000.0f);

} // ns glare

#include "glare/material/color_utils.hpp"

#include <glm/vec3.hpp>
#include <algorithm>
#include <cmath>

namespace glare {

glm::vec3 color_temp_to_rgb(float temp)
{
    glm::vec3 color;

    temp /= 100.0f;

    if (temp <= 66.0f) {
        color.r = 1.0f;
    } else {
        color.r = temp - 60.0f;
        color.r = 329.698727446 * (std::pow(color.r, -0.1332047592));
        color.r /= 255.0f;
    }

    if (temp <= 66.0f) {
        color.g = 99.4708025861 * std::log(temp) - 161.1195681661;
    } else {
        color.g = temp - 60.0f;
        color.g = 288.1221695283 * std::pow(color.g, -0.0755148492);
    }

    color.g /= 255.0f;

    if (temp >= 66.0f) {
        color.b = 1.0f;
    } else {
        color.b = temp - 10.0f;
        color.b = 138.5177312231 * std::log(color.b) - 305.0447927307;
        color.b /= 255.0f;
    }

    color.r = std::clamp(color.r, 0.0f, 1.0f);
    color.g = std::clamp(color.g, 0.0f, 1.0f);
    color.b = std::clamp(color.b, 0.0f, 1.0f);

    return color;
}

} // ns glare

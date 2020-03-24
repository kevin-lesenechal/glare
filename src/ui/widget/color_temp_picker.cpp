/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/ui/widget/color_temp_picker.hpp"
#include "glare/material/color_utils.hpp"

#include <imgui.h>
#include <glm/vec3.hpp>

namespace imgui = ImGui;

namespace glare::ui {

bool color_temp_picker(const char* label, float* value, float min, float max)
{
    imgui::BeginGroup();
    imgui::PushID(label);

    float width = imgui::CalcItemWidth();
    float height = imgui::GetFrameHeight();
    float spacing = imgui::GetStyle().ItemInnerSpacing.x;

    imgui::SetNextItemWidth(width - height - spacing);
    bool changed = imgui::SliderFloat("##temp", value, min, max, "%.0f K");

    glm::vec3 glm_color = color_temp_to_rgb(*value);
    ImVec4 color(glm_color.r, glm_color.g, glm_color.b, 1.0f);

    imgui::SameLine(0.0f, spacing);
    imgui::ColorButton("##preview", color);
    imgui::SameLine(0.0f, spacing);
    imgui::Text("%s", label);

    imgui::PopID();
    imgui::EndGroup();

    return changed;
}

} // ns glare::ui

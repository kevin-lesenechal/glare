/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/ui/light_config.hpp"
#include "glare/ui/widget/color_temp_picker.hpp"
#include "glare/material/color_utils.hpp"

#include <imgui.h>
#include <cmath>

namespace imgui = ImGui;

namespace glare {

LightConfig::LightConfig(const std::string& title,
                         const glm::vec3& position,
                         const glm::vec3& ambient,
                         const glm::vec3& diffuse,
                         const glm::vec3& specular)
  : position(position),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    m_title(title),
    m_color_temp(6000.0),
    m_color(color_temp_to_rgb(m_color_temp))
{
    update_colors();
}

void LightConfig::draw()
{
    bool update_out_colors = false;

    imgui::SetNextWindowBgAlpha(0.8f);
    if (!imgui::Begin(m_title.c_str())) {
        imgui::End();
        return;
    }

    imgui::AlignTextToFramePadding();
    imgui::InputFloat3("Position", &position.x);

    if (imgui::TreeNode("Automatic")) {

        if (ui::color_temp_picker("Color temp.", &m_color_temp)) {
            m_color           = color_temp_to_rgb(m_color_temp);
            update_out_colors = true;
        }
        if (imgui::ColorEdit3("Color", &m_color.r, ImGuiColorEditFlags_Float)) {
            m_color_temp      = NAN;
            update_out_colors = true;
        }

        update_out_colors |=
            imgui::SliderFloat("Intensity", &m_intensity, 0.0f, 10.0f);

        imgui::Separator();

        update_out_colors |=
            imgui::SliderFloat("Ambient factor", &m_ambient_factor, 0.0f, 1.0f)
            | imgui::SliderFloat("Diffuse factor", &m_diffuse_factor, 0.0f, 2.0f)
            | imgui::SliderFloat("Specular factor", &m_spec_factor, 0.0f, 5.0f);

        if (update_out_colors) {
            update_colors();
        }

        imgui::TreePop();
    }

    if (imgui::TreeNode("Manual")) {
        imgui::ColorEdit3("Ambient", &ambient.r, ImGuiColorEditFlags_Float);
        imgui::ColorEdit3("Diffuse", &diffuse.r, ImGuiColorEditFlags_Float);
        imgui::ColorEdit3("Specular", &specular.r, ImGuiColorEditFlags_Float);

        imgui::TreePop();
    }

    imgui::End();
}

void LightConfig::update_colors()
{
    ambient = m_color * m_intensity * m_ambient_factor;
    diffuse = m_color * m_intensity * m_diffuse_factor;
    specular = m_color * m_intensity * m_spec_factor;
}

} // ns glare

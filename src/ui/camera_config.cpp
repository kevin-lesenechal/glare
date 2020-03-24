/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/ui/camera_config.hpp"
#include "glare/camera.hpp"

#include <imgui.h>

namespace imgui = ImGui;

namespace glare {

CameraConfig::CameraConfig(Camera& camera)
  : m_camera(camera)
{}

void CameraConfig::draw()
{
    imgui::SetNextWindowBgAlpha(0.8f);
    if (!imgui::Begin("Camera")) {
        imgui::End();
        return;
    }

    bool update_view = false, update_proj = false;

    update_view |= imgui::InputFloat3("Position", &m_camera.position.x);
    update_proj |= imgui::SliderFloat("FOV", &m_camera.fov, 1.0f, 120.0f, "%.0f°");
    update_proj |= imgui::InputFloat("Aspect ratio", &m_camera.aspect_ratio);
    update_proj |= imgui::InputFloat("Z near", &m_camera.z_near);
    update_proj |= imgui::InputFloat("Z far", &m_camera.z_far);

    if (update_view) {
        m_camera.update_view_matrix();
    }
    if (update_proj) {
        m_camera.update_projection_matrix();
    }

    imgui::End();
}

} // ns glare

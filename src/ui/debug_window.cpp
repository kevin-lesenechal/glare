/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include <epoxy/gl.h>

#include "glare/ui/debug_window.hpp"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace imgui = ImGui;

namespace glare {

DebugWindow::DebugWindow()
  : m_prev_time(glfwGetTime()),
    m_wireframe("Wireframe", false, [](bool checked) {
        glPolygonMode(GL_FRONT_AND_BACK, checked ? GL_LINE : GL_FILL);
    }),
    m_vsync("V-Sync", true, [](bool checked){
        glfwSwapInterval(checked ? 1 : 0);
    }),
    m_culling("Face culling", true, [](bool checked) {
        if (checked) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    })
{}

void DebugWindow::draw()
{
    imgui::SetNextWindowBgAlpha(0.8f);
    if (!imgui::Begin("Debug")) {
        imgui::End();
        return;
    }

    double curr_time = glfwGetTime();
    double elapsed_time = curr_time - m_prev_time;
    m_prev_time = curr_time;

    imgui::Text("%.1f ms", elapsed_time * 1000.0f);
    imgui::SameLine(100.0f);
    imgui::Text("%.1f FPS", 1.0f / elapsed_time);

    m_wireframe.draw();
    m_vsync.draw();
    m_culling.draw();

    if (imgui::Button("Reload shaders")) {
        std::cout << "reload!" << std::endl;
    }

    imgui::End();
}

} // ns glare

/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/ui/ui_context.hpp"
#include "glare/opengl/context.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace imgui = ImGui;

namespace glare {

UiContext::UiContext(Context& context)
{
    IMGUI_CHECKVERSION();
    imgui::CreateContext();
    imgui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(context.glfw_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGuiIO& imgui_io = imgui::GetIO();
    imgui_io.IniFilename = "config/imgui.ini";
    imgui_io.Fonts->AddFontFromFileTTF("fonts/noto_sans_regular.ttf", 20.0f);
}

UiContext::~UiContext() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    imgui::DestroyContext();
}

void UiContext::frame_start()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    imgui::NewFrame();
}

void UiContext::frame_end()
{
    imgui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(imgui::GetDrawData());
}

} // ns glare

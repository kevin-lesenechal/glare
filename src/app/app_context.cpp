#include "app_context.hpp"

#include <stb_image.h>

AppContext::AppContext()
  : glare::Context(1280, 1024, "GLARE demo", true, []() {
        glfwWindowHint(GLFW_SAMPLES, 8);
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    })
{
    int icon_w, icon_h, channels;
    unsigned char* pixels = stbi_load("img/logo_32.png", &icon_w, &icon_h,
                                      &channels, 0);
    if (pixels == nullptr) {
        throw 42;
    }
    GLFWimage image{32, 32, pixels};
    glfwSetWindowIcon(m_window, 1, &image);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    glViewport(0, 0, 1280, 1024);
    glClearColor(0.224, 0.365, 0.569, 1.0);
}

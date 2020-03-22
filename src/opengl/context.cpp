/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include <epoxy/gl.h>

#include "glare/opengl/context.hpp"

#include <csignal>

namespace glare {

static void on_glfw_error(int, const char*)
{
    throw 42; // FIXME: proper exception
}

static void on_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity,
                        GLsizei, const GLchar* message, const void*)
{
    const char* src_str;
    const char* type_str;
    const char* sev_str;

    switch (source) {
        case GL_DEBUG_SOURCE_API: src_str = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src_str = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: src_str = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: src_str = "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: src_str = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER: src_str = "Other"; break;
        default: src_str = "Unknown source";
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR: type_str = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_str = "Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY: type_str = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: type_str = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER: type_str = "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP: type_str = "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP: type_str = "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER: type_str = "Other"; break;
        default: type_str = "Unknown type";
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: sev_str = "error"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: sev_str = "warning"; break;
        case GL_DEBUG_SEVERITY_LOW: sev_str = "notice"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: sev_str = "info"; break;
        default: sev_str = "unknown severity";
    }

    fprintf(
        stderr,
        "OpenGL [%s/%s] %s: %s (id=%u)\n",
        src_str, type_str, sev_str, message, id
    );

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        raise(SIGTRAP);
    }
}

static void on_glfw_window_resize(GLFWwindow* window, int width, int height)
{
    Context& context = *reinterpret_cast<Context*>(
        glfwGetWindowUserPointer(window)
    );
    if (context.on_window_resized) {
        context.on_window_resized(width, height);
    }
}

Context::Context(unsigned window_width,
                 unsigned window_height,
                 const std::string& window_title,
                 bool debug,
                 const std::function<void()>& pre_init)
  : m_window(nullptr)
{
    if (!glfwInit()) {
        throw 42; // FIXME: proper exception
    }

    glfwSetErrorCallback(&on_glfw_error);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    if (pre_init) {
        pre_init();
    }

    if ((m_window = glfwCreateWindow(
            static_cast<int>(window_width), static_cast<int>(window_height),
            window_title.c_str(),
            nullptr, nullptr
        )) == nullptr) {
        throw 42; // FIXME: proper exception
    }

    make_current();

    if (debug) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glDebugMessageCallback(&on_gl_error, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_NOTIFICATION,
                              0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_LOW,
                              0, nullptr, GL_TRUE);
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetWindowSizeCallback(m_window, &on_glfw_window_resize);
}

Context::~Context() noexcept
{
    glfwTerminate();
}

void Context::make_current()
{
    glfwMakeContextCurrent(m_window);
}

bool Context::is_running() const noexcept
{
    return !glfwWindowShouldClose(m_window);
}

void Context::swap_buffers()
{
    glfwSwapBuffers(m_window);
}

} // ns glare

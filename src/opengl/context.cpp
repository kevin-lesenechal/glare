/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include <epoxy/gl.h>

#include "glare/opengl/context.hpp"
#include "glare/opengl/extensions.hpp"

#include <csignal>

namespace sig2 = boost::signals2;

namespace glare {

static void on_glfw_error(int errc, const char* errmsg)
{
    throw std::runtime_error(
        "GLFW error: [" + std::to_string(errc) + "] " + errmsg
    );
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
                 const std::function<void()>& pre_init,
                 LoggerInterface& logger)
  : m_logger(logger),
    m_window(nullptr)
{
    if (!glfwInit()) {
        throw std::runtime_error("Couldn't initialize GLFW");
    }

    m_logger.debug("[Context] GLFW initialized");

    glfwSetErrorCallback(&on_glfw_error);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    if (pre_init) {
        m_logger.debug("[Context] Executing user pre-initialization...");
        pre_init();
    }

    m_logger.debug("[Context] Creating OpenGL context...");

    if ((m_window = glfwCreateWindow(
            static_cast<int>(window_width), static_cast<int>(window_height),
            window_title.c_str(),
            nullptr, nullptr
        )) == nullptr) {
        throw std::runtime_error("GLFW: Couldn't create the OpenGL context");
    }

    make_current();

    m_logger.info("[Context] OpenGL %s, %s",
                  glGetString(GL_VERSION), glGetString(GL_RENDERER));
    m_logger.info("[Context] GLSL %s",
                  glGetString(GL_SHADING_LANGUAGE_VERSION));

    if (debug) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glDebugMessageCallback(&on_gl_error, this);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_NOTIFICATION,
                              0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_LOW,
                              0, nullptr, GL_TRUE);
    }

    query_extensions();

    if (!ext::has_dsa) {
        m_logger.notice(
            "[Context] OpenGL context doesn't support Direct State Access"
        );
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetWindowSizeCallback(m_window, &on_glfw_window_resize);
    glfwSetKeyCallback(m_window, &on_glfw_key_event);
    glfwSetCursorPosCallback(m_window, &on_glfw_mouse_move);
    glfwSetScrollCallback(m_window, &on_glfw_mouse_scroll);
    glfwSetMouseButtonCallback(m_window, &on_glfw_mouse_button);
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

void Context::grab_cursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}

void Context::release_cursor()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }
}

void Context::query_extensions()
{
#define TEST(var, name) ext::var = epoxy_has_gl_extension("GL_" #name)

    TEST(has_dsa,                ARB_direct_state_access);
    TEST(has_tex_storage,        ARB_texture_storage); // GL 4.3
    TEST(has_anisotropic_filter, ARB_texture_filter_anisotropic); // GL 4.6

#undef TEST
}

sig2::connection Context::connect_keyboard_event(
    const KeyboardSignal::slot_type& slot)
{
    return m_keyboard_sigal.connect(slot);
}

sig2::connection Context::connect_mouse_move_event(
    const MouseMoveSignal::slot_type& slot)
{
    return m_mouse_move_signal.connect(slot);
}

sig2::connection Context::connect_mouse_scroll_event(
    const MouseScrollSignal::slot_type& slot)
{
    return m_mouse_scroll_signal.connect(slot);
}

sig2::connection Context::connect_mouse_button_event(
    const MouseButtonSignal::slot_type& slot)
{
    return m_mouse_button_signal.connect(slot);
}

void Context::on_glfw_key_event(GLFWwindow* window,
                                int key,
                                int scancode,
                                int action,
                                int)
{
    if (action == GLFW_REPEAT) {
        return;
    }

    Context& context = *reinterpret_cast<Context*>(
        glfwGetWindowUserPointer(window)
    );

    KeyboardEvent event;
    event.action = action == GLFW_PRESS ? KeyboardEvent::Action::Down
                                        : KeyboardEvent::Action::Up;
    event.key = static_cast<KeyboardEvent::Key>(key);
    event.scancode = scancode;

    context.m_keyboard_sigal(event);
}

void Context::on_glfw_mouse_move(GLFWwindow* window, double x, double y)
{
    Context& context = *reinterpret_cast<Context*>(
        glfwGetWindowUserPointer(window)
    );
    context.m_mouse_move_signal(MouseMoveEvent{x, y});
}

void Context::on_glfw_mouse_scroll(GLFWwindow* window, double x, double y)
{
    Context& context = *reinterpret_cast<Context*>(
        glfwGetWindowUserPointer(window)
    );
    context.m_mouse_scroll_signal(MouseScrollEvent{x, y});
}

void Context::on_glfw_mouse_button(GLFWwindow* window,
                                   int button,
                                   int action,
                                   int)
{
    Context& context = *reinterpret_cast<Context*>(
        glfwGetWindowUserPointer(window)
    );

    MouseButtonEvent event;
    event.action = action == GLFW_PRESS ? MouseButtonEvent::Action::Down
                                        : MouseButtonEvent::Action::Up;
    event.button = static_cast<MouseButtonEvent::Button>(button);

    context.m_mouse_button_signal(event);
}

void Context::on_gl_error(GLenum source,
                          GLenum type,
                          GLuint,
                          GLenum severity,
                          GLsizei,
                          const GLchar* message,
                          const void* priv)
{
    const char* src_str;
    const char* type_str;

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

    // We know for sure the source object is not actually const
    Context& context = *reinterpret_cast<Context*>(const_cast<void*>(priv));

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        context.m_logger.error("[OpenGL] %s/%s: %s", src_str, type_str, message);
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        context.m_logger.warning("[OpenGL] %s/%s: %s", src_str, type_str, message);
        break;

    case GL_DEBUG_SEVERITY_LOW:
        context.m_logger.notice("[OpenGL] %s/%s: %s", src_str, type_str, message);
        break;

    default:
        context.m_logger.info("[OpenGL] %s/%s: %s", src_str, type_str, message);
        break;
    }

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        raise(SIGTRAP);
    }
}

} // ns glare

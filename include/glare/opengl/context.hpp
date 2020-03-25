/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/null_logger.hpp"
#include "glare/input/keyboard_event.hpp"
#include "glare/input/mouse_move_event.hpp"
#include "glare/input/mouse_scroll_event.hpp"
#include "glare/input/mouse_button_event.hpp"

#include <GLFW/glfw3.h>
#include <boost/signals2.hpp>

#include <string>
#include <functional>

namespace glare {

class Context
{
public:
    template<typename T>
    using EventSignal = boost::signals2::signal<void(const T&)>;
    using KeyboardSignal = EventSignal<KeyboardEvent>;
    using MouseMoveSignal = EventSignal<MouseMoveEvent>;
    using MouseScrollSignal = EventSignal<MouseScrollEvent>;
    using MouseButtonSignal = EventSignal<MouseButtonEvent>;

    Context(unsigned window_width,
            unsigned window_height,
            const std::string& window_title,
            bool debug = false,
            const std::function<void()>& pre_init = std::function<void()>(),
            LoggerInterface& logger = null_logger);

    ~Context() noexcept;

    void make_current();
    void swap_buffers();

    void grab_cursor();
    void release_cursor();

    boost::signals2::connection connect_keyboard_event(
        const KeyboardSignal::slot_type& slot
    );

    boost::signals2::connection connect_mouse_move_event(
        const MouseMoveSignal::slot_type& slot
    );

    boost::signals2::connection connect_mouse_scroll_event(
        const MouseScrollSignal::slot_type& slot
    );

    boost::signals2::connection connect_mouse_button_event(
        const MouseButtonSignal::slot_type& slot
    );

    [[nodiscard]] GLFWwindow* glfw_window() const noexcept { return m_window; }
    [[nodiscard]] bool is_running() const noexcept;

public:
    std::function<void(unsigned new_width, unsigned new_height)> on_window_resized;

protected:
    LoggerInterface& m_logger;
    GLFWwindow* m_window;

private:
    void query_extensions();

    static void on_glfw_key_event(GLFWwindow* window,
                                  int key,
                                  int scancode,
                                  int action,
                                  int mods);

    static void on_glfw_mouse_move(GLFWwindow* window, double x, double y);

    static void on_glfw_mouse_scroll(GLFWwindow* window, double x, double y);

    static void on_glfw_mouse_button(GLFWwindow* window,
                                     int button,
                                     int action,
                                     int mods);

private:
    KeyboardSignal      m_keyboard_sigal;
    MouseMoveSignal     m_mouse_move_signal;
    MouseScrollSignal   m_mouse_scroll_signal;
    MouseButtonSignal   m_mouse_button_signal;
};

} // ns glare

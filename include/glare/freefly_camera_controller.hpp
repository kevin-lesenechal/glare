/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/input/keyboard_event.hpp"
#include "glare/input/mouse_move_event.hpp"
#include "glare/input/mouse_scroll_event.hpp"

#include <boost/signals2.hpp>

namespace glare {

class Context;
class Camera;

class FreeflyCameraController
{
public:
    struct KeyConfig {
        KeyboardEvent::Key forward  = KeyboardEvent::Key::W;
        KeyboardEvent::Key backward = KeyboardEvent::Key::S;
        KeyboardEvent::Key left     = KeyboardEvent::Key::A;
        KeyboardEvent::Key right    = KeyboardEvent::Key::D;
        KeyboardEvent::Key up       = KeyboardEvent::Key::Space;
        KeyboardEvent::Key down     = KeyboardEvent::Key::C;
        KeyboardEvent::Key run      = KeyboardEvent::Key::LeftShift;

        // Workaround for GCC bug 88165
        KeyConfig() noexcept {}
    };

    FreeflyCameraController(Context& context,
                            Camera& camera,
                            const KeyConfig& key_config = KeyConfig());

    void activate();
    void deactivate();

    void update(double time_delta);

public:
    float move_speed = 1.0f;
    float mouse_sensitivity = 0.3f;
    float run_factor = 5.0f;

private:
    enum MoveBit {
        Forward  = 1 << 0,
        Backward = 1 << 1,
        Left     = 1 << 2,
        Right    = 1 << 3,
        Up       = 1 << 4,
        Down     = 1 << 5
    };

    void on_keyboard_event(const KeyboardEvent& event);
    void on_mouse_move_event(const MouseMoveEvent& event);
    void on_mouse_scroll_event(const MouseScrollEvent& event);

private:
    Context&  m_context;
    Camera&   m_camera;
    KeyConfig m_key_config;
    unsigned  m_move_bits;
    bool      m_running;
    bool      m_first_move;
    double    m_prev_x;
    double    m_prev_y;
    boost::signals2::scoped_connection m_keyboard_conn;
    boost::signals2::scoped_connection m_mouse_move_conn;
    boost::signals2::scoped_connection m_mouse_scroll_conn;
};

} // ns glare

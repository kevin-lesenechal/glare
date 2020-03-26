/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/freefly_camera_controller.hpp"
#include "glare/opengl/context.hpp"
#include "glare/camera.hpp"

namespace glare {

FreeflyCameraController::FreeflyCameraController(Context& context,
                                                 Camera& camera,
                                                 const KeyConfig& key_config)
  : m_context(context),
    m_camera(camera),
    m_key_config(key_config),
    m_move_bits(0),
    m_running(false),
    m_first_move(true)
{}

void FreeflyCameraController::update(double time_delta)
{
    if (m_move_bits == 0u) {
        return;
    }

    float distance = move_speed * static_cast<float>(time_delta);
    if (m_running) {
        distance *= run_factor;
    }

    glm::vec3 move_dir(0.0f);

    if (m_move_bits & MoveBit::Forward) {
        move_dir += m_camera.front;
    }
    if (m_move_bits & MoveBit::Backward) {
        move_dir -= m_camera.front;
    }
    if (m_move_bits & MoveBit::Right) {
        move_dir += glm::cross(m_camera.front, m_camera.up);
    }
    if (m_move_bits & MoveBit::Left) {
        move_dir -= glm::cross(m_camera.front, m_camera.up);
    }
    if (m_move_bits & MoveBit::Up) {
        move_dir += m_camera.up;
    }
    if (m_move_bits & MoveBit::Down) {
        move_dir -= m_camera.up;
    }

    if (move_dir.x == 0.0f && move_dir.y == 0.0f && move_dir.z == 0.0f) {
        return;
    }

    m_camera.position += distance * glm::normalize(move_dir);
    m_camera.update_view_matrix();
}

void FreeflyCameraController::on_keyboard_event(const KeyboardEvent& event)
{
    MoveBit move_bit;

    if (event.key == m_key_config.forward) {
        move_bit = MoveBit::Forward;
    } else if (event.key == m_key_config.backward) {
        move_bit = MoveBit::Backward;
    } else if (event.key == m_key_config.left) {
        move_bit = MoveBit::Left;
    } else if (event.key == m_key_config.right) {
        move_bit = MoveBit::Right;
    } else if (event.key == m_key_config.up) {
        move_bit = MoveBit::Up;
    } else if (event.key == m_key_config.down) {
        move_bit = MoveBit::Down;
    } else if (event.key == m_key_config.run) {
        m_running = event.action == KeyboardEvent::Action::Down;
        return;
    } else {
        return;
    }

    if (event.action == KeyboardEvent::Action::Down) {
        m_move_bits |= move_bit;
    } else {
        m_move_bits &= ~move_bit;
    }

    // FIXME: lag on two strokes
    /*fprintf(stderr, "0x%.2x   %c %c %c %c\n",
            m_move_bits,
            (m_move_bits & MoveBit::Forward) ? 'Z' : '.',
            (m_move_bits & MoveBit::Backward) ? 'S' : '.',
            (m_move_bits & MoveBit::Left) ? 'Q' : '.',
            (m_move_bits & MoveBit::Right) ? 'D' : '.');*/
}

void FreeflyCameraController::on_mouse_move_event(const MouseMoveEvent& event)
{
    if (m_first_move) {
        m_prev_x = event.x;
        m_prev_y = event.y;
        m_first_move = false;
    }

    double dx = (m_prev_x - event.x) * mouse_sensitivity;
    double dy = (m_prev_y - event.y) * mouse_sensitivity;
    m_prev_x = event.x;
    m_prev_y = event.y;

    m_camera.yaw += static_cast<float>(dx);
    m_camera.pitch += static_cast<float>(dy);

    if (m_camera.pitch > 89.0f) {
        m_camera.pitch = 89.0f;
    } else if (m_camera.pitch < -89.0f) {
        m_camera.pitch = -89.0f;
    }

    m_camera.update_front_from_angles();
    m_camera.update_view_matrix();
}

void FreeflyCameraController::on_mouse_scroll_event(
    const MouseScrollEvent& event)
{
    float scroll_y = static_cast<float>(-event.y) * 3.0f;

    m_camera.fov = std::clamp(m_camera.fov + scroll_y, 1.0f, 120.0f);
    m_camera.update_projection_matrix();
}

void FreeflyCameraController::activate()
{
    m_keyboard_conn = m_context.connect_keyboard_event(
        [this](const KeyboardEvent& event) {
            on_keyboard_event(event);
        }
    );

    m_mouse_move_conn = m_context.connect_mouse_move_event(
        [this](const MouseMoveEvent& event) {
            on_mouse_move_event(event);
        }
    );

    m_mouse_scroll_conn = m_context.connect_mouse_scroll_event(
        [this](const MouseScrollEvent& event) {
            on_mouse_scroll_event(event);
        }
    );

    m_context.grab_cursor();
    m_first_move = true;
}

void FreeflyCameraController::deactivate()
{
    m_context.release_cursor();
    m_keyboard_conn.disconnect();
    m_mouse_move_conn.disconnect();
    m_mouse_scroll_conn.disconnect();
    m_move_bits = 0u;
}

} // ns glare

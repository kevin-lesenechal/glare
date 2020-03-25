/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace glare {

class Camera
{
public:
    Camera(const glm::vec3& position, float fov, float aspect_ratio);

    void update_view_matrix();
    void update_projection_matrix();
    void update_front_from_angles();

    [[nodiscard]] const glm::mat4& view_matrix() const noexcept
    { return m_view_mx; }

    [[nodiscard]] const glm::mat4& projection_matrix() const noexcept
    { return m_proj_mx; }

public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up = {0.0f, 0.0f, 1.0f};
    float     pitch;
    float     yaw;
    float     fov;
    float     aspect_ratio;
    float     z_near;
    float     z_far;

private:
    glm::mat4 m_view_mx;
    glm::mat4 m_proj_mx;
};

} // ns glare

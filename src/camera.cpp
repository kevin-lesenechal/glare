#include "glare/camera.hpp"

#include <glm/gtx/transform.hpp>

namespace glare {

Camera::Camera(const glm::vec3& position, float fov, float aspect_ratio)
  : position(position),
    front(1.0f, 0.0f, 0.0f),
    pitch(0.0f),
    yaw(0.0f),
    fov(fov),
    aspect_ratio(aspect_ratio),
    z_near(0.1f),
    z_far(100.0f)
{
    update_view_matrix();
    update_projection_matrix();
}

void Camera::update_view_matrix()
{
    m_view_mx = glm::lookAt(
        this->position,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
}

void Camera::update_projection_matrix()
{
    m_proj_mx = glm::perspective(
        glm::radians(this->fov),
        this->aspect_ratio,
        this->z_near,
        this->z_far
    );
}

} // ns glare

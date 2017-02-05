#include "glare/fps_camera.hpp"

namespace glare {

FpsCamera::FpsCamera(const glm::vec3& position, float fov, float aspect_ratio)
  : Camera(position, fov, aspect_ratio)
{}

void FpsCamera::update_position(unsigned move_bits, float time_delta)
{
    if (move_bits & MoveBit::Up) {
        //
    }
}

} // ns glare

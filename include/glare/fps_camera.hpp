#pragma once

#include "camera.hpp"

namespace glare {

class FpsCamera : public Camera
{
public:
    enum MoveBit {
        Up      = 1 << 0,
        Down    = 1 << 1,
        Left    = 1 << 2,
        Right   = 1 << 3
    };

    FpsCamera(const glm::vec3& position, float fov, float aspect_ratio);

    void update_position(unsigned move_bits, float time_delta);

private:
    float m_move_speed;
};

} // ns glare

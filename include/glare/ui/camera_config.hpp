#pragma once

namespace glare {

class Camera;

class CameraConfig
{
public:
    explicit CameraConfig(Camera& camera);

    void draw();

private:
    Camera& m_camera;
};

} // ns glare

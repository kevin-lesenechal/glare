/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

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

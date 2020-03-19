#pragma once

namespace glare {

struct Material;

class ShadingInterface
{
public:
    virtual ~ShadingInterface() = default;

    virtual void set_material(const Material& material) = 0;
};

}

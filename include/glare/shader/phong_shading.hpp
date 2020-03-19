#pragma once

#include "shading_interface.hpp"

#include <string>

namespace glare {

struct Material;
class ShaderProgram;

class PhongShading : public ShadingInterface
{
public:
    explicit PhongShading(ShaderProgram& program,
                          const std::string& material_var = "g_material");

    void set_material(const Material& material) override;

private:
    ShaderProgram&  m_program;
    std::string     m_material_var;
};

} // ns glare

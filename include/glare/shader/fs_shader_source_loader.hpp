#pragma once

#include "shader_source_loader_interface.hpp"

namespace glare {

class FsShaderSourceLoader : public ShaderSourceLoaderInterface
{
public:
    virtual std::string load_source(const std::string& location) override;
};

} // ns glare

#pragma once

#include <string>

namespace glare {

class ShaderSourceLoaderInterface
{
public:
    virtual ~ShaderSourceLoaderInterface() = default;

    virtual std::string load_source(const std::string& location) = 0;
};

} // ns glare

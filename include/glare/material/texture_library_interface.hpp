#pragma once

#include <string>
#include <memory>

namespace glare {

class Texture;

class TextureLibraryInterface
{
public:
    virtual ~TextureLibraryInterface() noexcept = default;

    virtual std::shared_ptr<Texture> get_texture(const std::string& name,
                                                 int unit) = 0;
};

} // ns glare

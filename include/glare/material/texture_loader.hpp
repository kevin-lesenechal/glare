#pragma once

#include <filesystem>

namespace glare {

class Texture;

class TextureLoader
{
public:
    Texture load_from_file(const std::filesystem::path& file_path,
                           int unit = 0);

private:
    Texture load_from_ktx_file(const std::filesystem::path& file_path,
                               int unit = 0);
};

} // ns glare

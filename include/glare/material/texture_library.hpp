/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "texture.hpp"
#include "glare/material/texture_library_interface.hpp"
#include "glare/material/texture_loader.hpp"
#include "glare/null_logger.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace glare {

class TextureLibrary : public TextureLibraryInterface
{
public:
    explicit TextureLibrary(TextureLoader& texture_loader,
                            LoggerInterface& logger = null_logger);

    void add_search_path(const std::filesystem::path& path);

    [[nodiscard]]
    std::shared_ptr<Texture> get_texture(const std::string& name) override;

    [[nodiscard]]
    std::filesystem::path resolve_texture_path(const std::string& name);

private:
    LoggerInterface& m_logger;
    TextureLoader& m_texture_loader;
    std::unordered_map<std::string, std::weak_ptr<Texture>> m_textures;
    std::vector<std::filesystem::path> m_search_paths;
};

} // ns glare

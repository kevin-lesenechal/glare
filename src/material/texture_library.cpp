/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/material/texture_library.hpp"

namespace fs = std::filesystem;

namespace glare {

TextureLibrary::TextureLibrary(TextureLoader& texture_loader,
                               LoggerInterface& logger)
  : m_logger(logger),
    m_texture_loader(texture_loader)
{}

void TextureLibrary::add_search_path(const fs::path& path)
{
    m_logger.debug("[TextureLibrary] New search path \"%s\"", path.c_str());
    m_search_paths.push_back(path);
}

std::shared_ptr<Texture> TextureLibrary::get_texture(
    const std::string& name,
    int unit
) {
    m_logger.debug(
        "[TextureLibrary] Looking for texture \"%s\" (unit %d)", name.c_str(), unit
    );
    auto it = m_textures.find(name);

    if (it != m_textures.end()) {
        if (std::shared_ptr<Texture> texture = it->second.lock()) {
            if (texture->unit() != unit) {
                throw 42;
            }

            m_logger.debug("[TextureLibrary] Already loaded");
            return texture;
        }
    }

    fs::path tex_path = resolve_texture_path(name);
    m_logger.debug("[TextureLibrary] Loading texture at \"%s\"", tex_path.c_str());
    auto texture = std::make_shared<Texture>(
        m_texture_loader.load_from_file(tex_path, unit)
    );

    m_textures.insert_or_assign(name, texture);

    return texture;
}

fs::path TextureLibrary::resolve_texture_path(const std::string& name)
{
    for (const auto& search_path : m_search_paths) {
        fs::path tex_path = search_path / name;

        if (fs::exists(tex_path)) {
            if (!fs::is_regular_file(tex_path)) {
                throw 42;
            }

            return tex_path;
        }
    }

    throw 42;
}

} // ns glare

/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/shader/fs_shader_source_loader.hpp"

#include <fstream>

namespace fs = std::filesystem;

namespace glare {

std::string FsShaderSourceLoader::load_source(const std::string& name)
{
    fs::path shader_path = resolve_shader_path(name);

    std::ifstream file(shader_path);
    file.exceptions(std::ios::failbit);

    if (!file) {
        throw std::runtime_error(
            "Couldn't open shader file \"" + shader_path.string() + "\""
        );
    }

    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

fs::path FsShaderSourceLoader::resolve_shader_path(const std::string& name)
{
    for (const auto& search_path : m_search_paths) {
        fs::path shader_path = search_path / name;

        if (fs::exists(shader_path)) {
            if (!fs::is_regular_file(shader_path)) {
                throw std::runtime_error(
                    "Shader \"" + shader_path.string() + "\" is not a regular "
                    "file"
                );
            }

            return shader_path;
        }
    }

    throw std::runtime_error(
        "FsShaderSourceLoader: couldn't find any shader file for "
        "\"" + name + "\""
    );
}

void FsShaderSourceLoader::add_search_path(const std::filesystem::path& path)
{
    if (!fs::is_directory(path)) {
        throw std::runtime_error(
            "Shader search path \"" + path.string() + "\" is not a directory"
        );
    }

    m_search_paths.push_back(path);
}

} // ns glare

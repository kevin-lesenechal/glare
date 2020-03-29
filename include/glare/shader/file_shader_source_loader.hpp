/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "shader_source_loader_interface.hpp"

#include <filesystem>
#include <vector>

namespace glare {

class FileShaderSourceLoader : public ShaderSourceLoaderInterface
{
public:
    /**
     * @brief Load GLSL source from a shader name
     * @param name A symbolic name (e.g. "foo/bar.glsl")
     * @return The entire source file content
     */
    [[nodiscard]]
    std::string load_source(const std::string& name) override;

    /**
     * @brief Find a shader file path from its name
     * @param name The shader symbolic name
     * @return The shader file's path
     */
    [[nodiscard]]
    std::filesystem::path resolve_shader_path(const std::string& name);

    /**
     * @brief Add a directory to the list of searched directories
     * @param path The directory path
     */
    void add_search_path(const std::filesystem::path& path);

private:
    /// The list of search directories
    std::vector<std::filesystem::path> m_search_paths;
};

} // ns glare

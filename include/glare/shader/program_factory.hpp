/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <filesystem>

namespace glare {

class ShaderSourceLoaderInterface;
class ShaderPreprocessor;
class ShaderProgram;

class ProgramFactory
{
public:
    ProgramFactory(ShaderSourceLoaderInterface& source_loader,
                   ShaderPreprocessor& preprocessor);

    ShaderProgram make(const std::filesystem::path& vert_shader_file,
                       const std::filesystem::path& frag_shader_file);

private:
    ShaderSourceLoaderInterface& m_source_loader;
    ShaderPreprocessor& m_preprocessor;
};

} // ns glare

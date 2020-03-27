/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/shader/shader_source_loader_interface.hpp"
#include "glare/shader/shader_preprocessor.hpp"
#include "glare/shader/program_factory.hpp"
#include "glare/shader/shader.hpp"
#include "glare/shader/shader_program.hpp"

namespace fs = std::filesystem;

namespace glare {

ProgramFactory::ProgramFactory(ShaderSourceLoaderInterface& source_loader,
                               ShaderPreprocessor& preprocessor)
  : m_source_loader(source_loader),
    m_preprocessor(preprocessor)
{}

ShaderProgram ProgramFactory::make(const fs::path& vert_shader_file,
                                   const fs::path& frag_shader_file)
{
    Shader vert_shader(Shader::Type::Vertex);
    vert_shader.load_source(
        m_preprocessor.preprocess(
            m_source_loader.load_source(vert_shader_file)
        )
    );

    Shader frag_shader(Shader::Type::Fragment);
    frag_shader.load_source(
        m_preprocessor.preprocess(
            m_source_loader.load_source(frag_shader_file)
        )
    );

    ShaderProgram program({&vert_shader, &frag_shader});
    program.link();

    return program;
}

} // ns glare

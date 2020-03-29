/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/shader/shader_program_library.hpp"
#include "glare/shader/file_shader_source_loader.hpp"
#include "glare/platform/file_watcher_interface.hpp"

namespace fs = std::filesystem;

namespace glare {

ShaderProgramLibrary::ShaderProgramLibrary(
    ShaderPreprocessor& preprocessor,
    FileShaderSourceLoader& source_loader,
    FileWatcherInterface& file_watcher,
    LoggerInterface& logger)
  : m_preprocessor(preprocessor),
    m_source_loader(source_loader),
    m_file_watcher(file_watcher),
    m_logger(logger)
{}

AutoProgram& ShaderProgramLibrary::get_program(const std::string& name)
{
    return m_programs.at(name);
}

void ShaderProgramLibrary::register_program(
    const std::string& name,
    const std::string& vert_name,
    const std::string& frag_name)
{
    std::map<Shader::Type, std::string> shader_names = {
        {Shader::Type::Vertex, vert_name},
        {Shader::Type::Fragment, frag_name},
    };

    AutoProgram program(
        shader_names,
        m_preprocessor,
        m_source_loader,
        m_logger
    );

    auto res = m_programs.insert({name, std::move(program)});
    AutoProgram& prog_ref = res.first->second;

    for (const auto& kv : shader_names) {
        fs::path path = m_source_loader.resolve_shader_path(kv.second);
        m_file_watcher.add_watch(path, FileWatcherInterface::Event::Modified)
            .connect([&prog_ref]() {
                prog_ref.reload();
            });
    }
}

void ShaderProgramLibrary::reload_program(AutoProgram& program,
                                          const std::string& name)
{
    try {
        program.reload();
    } catch (const std::exception& e) {
        m_logger.error(
            "[AutoProgram] %s: reload error: %s",
            name.c_str(), e.what()
        );
        return;
    }

    m_logger.info(
        "[AutoProgram] Program \"%s\" reloaded",
        name.c_str()
    );
}

} // ns glare

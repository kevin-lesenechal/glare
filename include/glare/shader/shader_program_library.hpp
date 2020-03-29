/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/shader/autoprogram.hpp"
#include "glare/null_logger.hpp"

#include <string>
#include <unordered_map>
#include <filesystem>

namespace glare {

class ShaderPreprocessor;
class FileShaderSourceLoader;
class FileWatcherInterface;

class ShaderProgramLibrary
{
    struct Definition {
        std::filesystem::path vert_file;
        std::filesystem::path frag_file;
    };

public:
    ShaderProgramLibrary(ShaderPreprocessor& preprocessor,
                         FileShaderSourceLoader& source_loader,
                         FileWatcherInterface& file_watcher,
                         LoggerInterface& logger = null_logger);

    AutoProgram& get_program(const std::string& name);

    void register_program(const std::string& name,
                          const std::string& vert_name,
                          const std::string& frag_name);

private:
    void reload_program(AutoProgram& program, const std::string& name);

private:
    ShaderPreprocessor& m_preprocessor;
    FileShaderSourceLoader& m_source_loader;
    FileWatcherInterface& m_file_watcher;
    LoggerInterface& m_logger;

    std::unordered_map<std::string, AutoProgram> m_programs;
};

} // ns glare

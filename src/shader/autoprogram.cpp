/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/shader/autoprogram.hpp"

namespace glare {

AutoProgram::AutoProgram(std::map<Shader::Type, std::string> shader_names,
                         ShaderPreprocessor& shader_preprocessor,
                         ShaderSourceLoaderInterface& source_loader,
                         LoggerInterface& logger)
  : m_shader_names(std::move(shader_names)),
    m_shader_preprocessor(shader_preprocessor),
    m_source_loader(source_loader),
    m_logger(logger)
{}

void AutoProgram::use()
{
    if (m_dirty) {
        reload();
    }

    ShaderProgram::use();
}

void AutoProgram::reload()
{
    ShaderProgram new_program;

    for (const auto& kv : m_shader_names) {
        try {
            Shader shader(kv.first);
            shader.load_source(
                m_shader_preprocessor.preprocess(
                    m_source_loader.load_source(kv.second),
                    m_defines
                )
            );
            shader.compile();
            new_program.add_shader(shader);
        } catch (const std::exception& e) {
            m_logger.error(
                "[AutoProgram] Shader reload error \"%s\": %s",
                kv.second.c_str(),
                e.what()
            );
            return;
        }
    }

    try {
        new_program.link();
    } catch (const std::exception& e) {
        m_logger.error(
            "[AutoProgram] Reload error: %s",
            e.what()
        );
        return;
    }

    ShaderProgram::operator=(std::move(new_program));
    m_dirty = false;
}

void AutoProgram::define(const std::string& key)
{
    define_expr(key, "");
}

void AutoProgram::define_expr(const std::string& key,
                                     const std::string& expr)
{
    m_defines[key] = expr;
    m_dirty = true;
}

void AutoProgram::undefine(const std::string& key)
{
    m_defines.erase(key);
    m_dirty = true;
}

} // ns glare

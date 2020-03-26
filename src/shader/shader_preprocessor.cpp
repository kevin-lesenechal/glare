/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/shader/shader_preprocessor.hpp"
#include "glare/shader/shader_source_loader_interface.hpp"

#include <regex>

namespace glare {

ShaderPreprocessor::ShaderPreprocessor(
    ShaderSourceLoaderInterface& source_loader,
    LoggerInterface& logger)
  : m_logger(logger),
    m_source_loader(source_loader)
{}

std::string ShaderPreprocessor::preprocess(const std::string& source)
{
    std::istringstream ss(source);
    std::string line;
    std::string output;
    std::regex version_regex("^\\s*#version\\s");
    std::regex include_regex(R"(^\s*#include ["<](.*)[">]\s*$)");
    bool version_found = false;
    unsigned line_no = 0;

    while (std::getline(ss, line)) {
        ++line_no;

        std::smatch matches;
        if (std::regex_search(line, matches, include_regex)) {
            std::string include_file = matches[1].str();
            m_logger.debug("include file '%s'", include_file.c_str());
            std::string include_source
                = m_source_loader.load_source(include_file);

            if (include_source.empty()
                || include_source[include_source.size() - 1] != '\n') {
                throw std::runtime_error(
                    "Expecting text file for GLSL file \"" + include_file
                    + "\", last line is not terminated: GLARE requires POSIX-"
                      "conformant text files"
                );
            }

            output += "#line 1\n";
            output += std::regex_replace(include_source, version_regex, "//$&");
            output += "#line " + std::to_string(line_no + 1) + "\n";
            continue;
        }

        output += line + "\n";

        if (std::regex_search(line, version_regex)) {
            if (version_found) {
                throw std::runtime_error("Multiple '#version' found");
            }

            for (const auto& pair : m_defines) {
                if (pair.second.empty()) {
                    output += "#define " + pair.first + "\n";
                } else {
                    output +=
                        "#define " + pair.first + " " + pair.second + "\n";
                }
            }

            if (!m_defines.empty()) {
                output += "#line " + std::to_string(line_no + 1) + "\n";
            }
            version_found = true;
        }
    }

    if (!version_found) {
        throw std::runtime_error("Couldn't find mandatory '#version'");
    }

    return output;
}

void ShaderPreprocessor::define(const std::string& key)
{
    define_expr(key, "");
}

void ShaderPreprocessor::define_expr(const std::string& key,
                                     const std::string& expr)
{
    m_defines[key] = expr;
}

void ShaderPreprocessor::undefine(const std::string& key)
{
    m_defines.erase(key);
}

} // ns glare

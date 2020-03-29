/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/null_logger.hpp"
#include "glsl_types.hpp"

#include <string>
#include <map>
#include <glm/fwd.hpp>

namespace glare {

class ShaderSourceLoaderInterface;

/**
 * @brief Preprocess GLSL source files before handing them for compilation
 *
 * This GLSL preprocessor will apply transformations to a GLSL source given as
 * string. The preprocessing pipeline includes:
 *
 *  * Defining macros with specific values at the very beginning of the shader,
 *    typically to pass configuration variables as defines, allowing the use of
 *    @c #ifdef (etc.) directives in GLSL based on these variables;
 *  * Including other GLSL files using a @c #include directive, taking care of
 *    preserving file names and line numbers for easy debugging.
 */
class ShaderPreprocessor
{
public:
    /**
     * @param source_loader A GLSL source loader to load include files
     * @param logger        An optional logger instance
     */
    explicit ShaderPreprocessor(ShaderSourceLoaderInterface& source_loader,
                                LoggerInterface& logger = null_logger);

    /**
     * @brief Returns a preprocessed version of @p source ready for compilation
     * @param source A GLSL source containing preprocessing directives
     * @return The preprocessed GLSL source ready to compile
     */
    std::string preprocess(const std::string& source,
                           const std::map<std::string, std::string>& defines);

private:
    LoggerInterface& m_logger;

    /// A GLSL source loader to load include files
    ShaderSourceLoaderInterface& m_source_loader;
};

} // ns glare

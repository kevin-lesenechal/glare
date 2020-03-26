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

class ShaderPreprocessor
{
public:
    explicit ShaderPreprocessor(ShaderSourceLoaderInterface& source_loader,
                                LoggerInterface& logger = null_logger);

    std::string preprocess(const std::string& source);

    /**
     * @brief Define a macro with a specific value at the source's beginning
     * @param key   The macro name
     * @param value The macro value
     *
     * Add a macro definition at the beginning and the input GLSL source, just
     * after the @c #version directive, with a specific value. This is
     * typically used to dynamically configure the shaders based on the user's
     * configuration (enabling or disabling some rendering features for
     * example). The @p value is passed through @c glsl_literal_value() to
     * determine the GLSL literal expression. Macro definitions are sorted
     * lexicographically in the GLSL output.
     */
    template<typename T>
    void define(const std::string& key, const T& value)
    {
        define_expr(key, glsl_literal_value(value));
    }

    /**
     * @brief Define a macro without a value
     * @param key The macro name
     */
    void define(const std::string& key);

    /**
     * @brief Define a macro with a specifc GLSL expression
     * @param key The macro name
     * @param expr The GLSL expression to initialize the macro
     */
    void define_expr(const std::string& key, const std::string& expr);

    /**
     * @brief Undefine a previously defined user macro
     * @param key The macro name to undefine
     */
    void undefine(const std::string& key);

private:
    LoggerInterface& m_logger;
    ShaderSourceLoaderInterface& m_source_loader;

    /// The list of user-defined macro definitions
    std::map<std::string, std::string> m_defines;
};

} // ns glare

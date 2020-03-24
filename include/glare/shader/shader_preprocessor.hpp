/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/null_logger.hpp"

#include <string>
#include <map>

namespace glare {

class ShaderSourceLoaderInterface;

class ShaderPreprocessor
{
public:
    explicit ShaderPreprocessor(ShaderSourceLoaderInterface& source_loader,
                                LoggerInterface& logger = null_logger);

    std::string preprocess(const std::string& source);

    void define(const std::string& key, const std::string& value);
    void undefine(const std::string& key);

private:
    LoggerInterface& m_logger;
    ShaderSourceLoaderInterface& m_source_loader;
    std::map<std::string, std::string> m_defines;
};

} // ns glare

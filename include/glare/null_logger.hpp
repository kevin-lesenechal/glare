/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "abstract_logger.hpp"

namespace glare {

class NullLogger : public AbstractLogger
{
public:
    void log(Severity, const char*, std::va_list) const override
    {}
};

inline NullLogger null_logger;

} // ns glare

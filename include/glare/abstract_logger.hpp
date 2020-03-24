/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "logger_interface.hpp"

namespace glare {

class AbstractLogger : public LoggerInterface
{
public:
    void debug(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Debug, fmesg, args);
        va_end(args);
    }

    void info(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Info, fmesg, args);
        va_end(args);
    }

    void notice(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Notice, fmesg, args);
        va_end(args);
    }

    void warning(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Warning, fmesg, args);
        va_end(args);
    }

    void error(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Error, fmesg, args);
        va_end(args);
    }

    void critical(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Critical, fmesg, args);
        va_end(args);
    }

    void alert(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Alert, fmesg, args);
        va_end(args);
    }

    void emergency(const char* fmesg, ...) const override
    {
        std::va_list args;
        va_start(args, fmesg);
        log(Severity::Emergency, fmesg, args);
        va_end(args);
    }
};

} // ns glare

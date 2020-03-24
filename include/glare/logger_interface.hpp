/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <cstdarg>

namespace glare {

#define FMT_ATTR __attribute__((format(printf, 2, 3)))

class LoggerInterface
{
public:
    enum class Severity {
        Debug       = -1, ///< Detailed debug-level message
        Info        = 0,  ///< Informational message, interesting events
        Notice      = 1,  ///< Normal but significant message
        Warning     = 2,  ///< Warning of abnormal condition
        Error       = 3,  ///< Runtime error message
        Critical    = 4,  ///< Critical condition, component unavailable
        Alert       = 5,  ///< Alert condition, action must be taken
        Emergency   = 6   ///< Extreme condition, urgent action must be taken
    };

    virtual ~LoggerInterface() noexcept = default;

    virtual void debug(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void info(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void notice(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void warning(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void error(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void critical(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void alert(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void emergency(const char* fmesg, ...) const FMT_ATTR = 0;

    virtual void log(Severity severity,
                     const char* fmesg,
                     std::va_list args) const = 0;
};

#undef FMT_ATTR

} // ns glare

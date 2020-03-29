/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include <filesystem>
#include <boost/signals2.hpp>

namespace glare {

class FileWatcherInterface
{
public:
    using WatchSignal = boost::signals2::signal<void()>;

    enum class Event {
        Modified
    };

    virtual ~FileWatcherInterface() noexcept = default;

    virtual WatchSignal& add_watch(const std::filesystem::path& file_path,
                                   Event event) = 0;

    virtual void poll() = 0;
};

} // ns glare

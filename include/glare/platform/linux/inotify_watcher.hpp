/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

#include "glare/platform/file_watcher_interface.hpp"

#include <unordered_map>

namespace glare::platform {

class InotifyWatcher : public FileWatcherInterface
{
public:
    InotifyWatcher();

    ~InotifyWatcher() noexcept override;

    WatchSignal& add_watch(const std::filesystem::path& file_path,
                           Event event) override;

    void poll() override;

private:
    int m_fd;
    std::unordered_map<int, WatchSignal> m_signals;
};

} // ns glare

/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#include "glare/platform/linux/inotify_watcher.hpp"

#include <sys/inotify.h>
#include <unistd.h>

namespace glare::platform {

InotifyWatcher::InotifyWatcher()
{
    if ((m_fd = inotify_init1(IN_NONBLOCK)) == -1) {
        throw std::system_error(
            errno, std::system_category(),
            "Coulnd't initialize inotify instance for file watcher"
        );
    }
}

InotifyWatcher::~InotifyWatcher() noexcept
{
    close(m_fd);
}

FileWatcherInterface::WatchSignal& InotifyWatcher::add_watch(
    const std::filesystem::path& file_path,
    Event event)
{
    int wd;
    int flags = 0;

    switch (event) {
    case Event::Modified: flags = IN_CLOSE_WRITE; break;
    default:
        throw std::logic_error("Not implemented");
    }

    if ((wd = inotify_add_watch(m_fd, file_path.c_str(), flags)) == -1) {
        throw std::system_error(
            errno, std::system_category(),
            "Couldn't watch file \"" + file_path.string() + "\""
        );
    }

    auto res = m_signals.insert({wd, WatchSignal()});

    return res.first->second;
}

void InotifyWatcher::poll()
{
    for (;;) {
        char buff[4096];
        ssize_t size_read = read(m_fd, buff, sizeof buff);

        if (size_read == -1 && errno != EAGAIN) {
            throw std::system_error(
                errno, std::system_category(),
                "Couldn't read inotify file descriptor"
            );
        } else if (size_read <= 0) {
            break;
        }

        const inotify_event* event = nullptr;

        for (char* ptr = buff; ptr < buff + size_read;
             ptr += sizeof *event + event->len) {
            event = reinterpret_cast<inotify_event*>(ptr);

            m_signals.at(event->wd)();
        }
    }
}

} // ns glare

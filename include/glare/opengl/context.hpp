#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <functional>

namespace glare {

class Context
{
public:
    Context(unsigned window_width,
            unsigned window_height,
            const std::string& window_title,
            bool debug = false,
            const std::function<void()>& pre_init = std::function<void()>());

    ~Context() noexcept;

    void make_current();
    void swap_buffers();

    [[nodiscard]] GLFWwindow* glfw_window() const noexcept { return m_window; }
    [[nodiscard]] bool is_running() const noexcept;

public:
    std::function<void(unsigned new_width, unsigned new_height)> on_window_resized;

protected:
    GLFWwindow* m_window;
};

} // ns glare

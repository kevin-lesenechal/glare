#pragma once

#include <functional>

namespace glare {

class Checkbox
{
public:
    Checkbox(const char* label, bool checked, std::function<void(bool)> callback);

    void draw();

private:
    const char* m_label;
    bool m_checked;
    std::function<void(bool)> m_callback;
};

} // ns glare

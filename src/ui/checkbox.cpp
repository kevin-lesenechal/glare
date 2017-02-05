#include "glare/ui/checkbox.hpp"

#include <imgui/imgui.h>

namespace imgui = ImGui;

namespace glare {

Checkbox::Checkbox(const char* label, bool checked,
                   std::function<void(bool)> callback)
  : m_label(label),
    m_checked(checked),
    m_callback(callback)
{}

void Checkbox::draw()
{
    bool prev_checked = m_checked;

    imgui::Checkbox(m_label, &m_checked);
    if (prev_checked != m_checked) {
        m_callback(m_checked);
    }
}

} // ns glare

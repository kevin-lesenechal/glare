#include "glare/ui/light_config.hpp"

#include <imgui/imgui.h>

namespace imgui = ImGui;

namespace glare {

LightConfig::LightConfig(const std::string& title,
                         const glm::vec3& position,
                         const glm::vec3& ambient,
                         const glm::vec3& diffuse,
                         const glm::vec3& specular)
  : m_title(title),
    position(position),
    ambient(ambient),
    diffuse(diffuse),
    specular(specular)
{}

void LightConfig::draw()
{
    imgui::SetNextWindowBgAlpha(0.8f);
    if (!imgui::Begin(m_title.c_str())) {
        imgui::End();
        return;
    }

    imgui::AlignTextToFramePadding();
    imgui::InputFloat3("Position", &position.x);
    imgui::ColorEdit3("Ambient", &ambient.r, ImGuiColorEditFlags_Float);
    imgui::ColorEdit3("Diffuse", &diffuse.r, ImGuiColorEditFlags_Float);
    imgui::ColorEdit3("Specular", &specular.r, ImGuiColorEditFlags_Float);

    imgui::End();
}

} // ns glare

//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#include "Panel.h"

#include <imgui.h>

void Panel::draw(const sf::FloatRect& area, const UIData& data, UIActions& actions) {
    ImGui::SetNextWindowPos({area.position.x, area.position.y}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({area.size.x, area.size.y}, ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

    // Tanka linija oko svakog panela: susedni paneli time dobijaju razdvajanje.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{0.20f, 0.24f, 0.34f, 1.f});
    if (background) ImGui::PushStyleColor(ImGuiCol_WindowBg, *background);

    ImGui::Begin(name.c_str(), nullptr, flags);
    drawContent(data, actions);
    ImGui::End();

    if (background) ImGui::PopStyleColor();
    ImGui::PopStyleColor();      // Border
    ImGui::PopStyleVar(2);
}

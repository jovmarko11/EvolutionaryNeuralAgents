//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#include "TransportPanel.h"

#include <cstdio>
#include <imgui.h>

namespace {
    // Semanticke boje ishoda (iste ce biti i u grafikonima).
    constexpr ImVec4 aliveColor   {1.00f, 0.42f, 0.37f, 1.f};
    constexpr ImVec4 reachedColor {0.24f, 0.86f, 0.59f, 1.f};
    constexpr ImVec4 crashedColor {1.00f, 0.36f, 0.42f, 1.f};
    constexpr ImVec4 timeoutColor {0.95f, 0.72f, 0.29f, 1.f};

    void counter(const char* label, int value, const ImVec4& color) {
        ImGui::TextColored(color, "%s %d", label, value);
    }
}

void TransportPanel::drawContent(const UIData& data, UIActions& actions) {
    const TransportInfo& t = data.transport;

    // --- red 1: kontrole ---
    if (ImGui::Button(t.paused ? "Play" : "Pause", {70, 0})) {
        actions.togglePause = true;
    }

    ImGui::SameLine();
    ImGui::BeginDisabled(!t.paused);          // Step ima smisla samo u pauzi
    if (ImGui::Button("Step", {60, 0})) {
        actions.step = true;
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Speed");
    ImGui::SameLine();

    float speed = static_cast<float>(t.visibleSpeed);
    ImGui::SetNextItemWidth(180);
    if (ImGui::SliderFloat("##speed", &speed, 0.1f, 20.0f, "%.1fx", ImGuiSliderFlags_Logarithmic)) {
        actions.setVisibleSpeed = speed;
    }

    ImGui::SameLine(0, 24);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Generation %d", t.generation);

    // --- red 2: korak epizode + brojaci ---
    const float fraction = t.maxSteps > 0 ? static_cast<float>(t.step) / static_cast<float>(t.maxSteps) : 0.f;
    char overlay[32];
    std::snprintf(overlay, sizeof(overlay), "step %d / %d", t.step, t.maxSteps);
    ImGui::ProgressBar(fraction, {260, 0}, overlay);

    ImGui::SameLine(0, 24);
    counter("alive",   t.alive,   aliveColor);
    ImGui::SameLine(0, 16);
    counter("reached", t.reached, reachedColor);
    ImGui::SameLine(0, 16);
    counter("crashed", t.crashed, crashedColor);
    ImGui::SameLine(0, 16);
    counter("timeout", t.timeout, timeoutColor);
}

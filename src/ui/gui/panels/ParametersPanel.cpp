#include "ParametersPanel.h"

#include <imgui.h>

namespace {
    constexpr ImVec4 changedColor{1.00f, 0.62f, 0.20f, 1.f};

    // Slider koji se oboji narandzasto kad se razlikuje od reference.
    template <typename T>
    void field(const char* label, T& value, const T& reference, T min, T max, const char* format,
               ImGuiSliderFlags flags = 0) {
        const bool changed = value != reference;
        if (changed) ImGui::PushStyleColor(ImGuiCol_Text, changedColor);

        constexpr ImGuiDataType type = std::is_same_v<T, double> ? ImGuiDataType_Double : ImGuiDataType_S32;
        ImGui::SetNextItemWidth(120);
        ImGui::SliderScalar(label, type, &value, &min, &max, format, flags);

        if (changed) ImGui::PopStyleColor();
    }
}

void ParametersPanel::syncWithProgram(const ParametersInfo& info) {
    if (!initialized) {
        draft = reference = info.applied;
        initialized = true;
        return;
    }
    // Dok korisnik nista ne menja (i nema zahteva na cekanju), draft prati program.
    if (!info.pending && draft == reference) {
        draft = reference = info.applied;
    }
}

void ParametersPanel::drawContent(const UIData& data, UIActions& actions) {
    const ParametersInfo& info = data.parameters;
    syncWithProgram(info);

    drawGaSection(info.populationSize);
    ImGui::Spacing();
    drawFitnessSection();
    ImGui::Spacing();

    // --- Apply / Revert ---
    const bool dirty = !(draft == reference);
    const bool differsFromApplied = !(draft == info.applied);

    ImGui::BeginDisabled(!dirty);
    if (ImGui::Button("Apply", {90, 0})) {
        actions.applyTraining = draft;
        reference = draft;
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    ImGui::BeginDisabled(!differsFromApplied && !dirty);
    if (ImGui::Button("Revert", {90, 0})) {
        draft = reference = info.applied;
    }
    ImGui::EndDisabled();

    if (info.pending) {
        ImGui::TextColored(changedColor, "Pending: applies at next generation");
    } else if (dirty) {
        ImGui::TextDisabled("Unapplied changes");
    }

}

void ParametersPanel::drawGaSection(int populationSize) {
    if (!ImGui::CollapsingHeader("Genetic algorithm", ImGuiTreeNodeFlags_DefaultOpen)) return;

    GAParams& p = draft.ga;
    const GAParams& r = reference.ga;
    const int maxTournament = populationSize > 0 ? populationSize : 1;

    field("Elitism",           p.elitismRatio,      r.elitismRatio,      0.0, 0.5, "%.3f");
    field("Mutation ratio",    p.mutationRatio,     r.mutationRatio,     0.0, 1.0, "%.3f");
    field("Mutation strength", p.mutationStrength,  r.mutationStrength,  0.0, 1.0, "%.3f");
    field("Tournament size",   p.tournamentSize,    r.tournamentSize,    1,   maxTournament, "%d");
}

void ParametersPanel::drawFitnessSection() {
    if (!ImGui::CollapsingHeader("Fitness weights", ImGuiTreeNodeFlags_DefaultOpen)) return;

    FitnessWeights& w = draft.fitness;
    const FitnessWeights& r = reference.fitness;

    field("Progress",  w.progress,  r.progress,  0.0, 50.0,   "%.2f");
    field("Survival",  w.survival,  r.survival,  0.0, 0.2,    "%.3f");
    field("Goal",      w.goal,      r.goal,      0.0, 5000.0, "%.0f");
    field("Collision", w.collision, r.collision, 0.0, 500.0,  "%.1f");
    field("Speed",     w.speed,     r.speed,     0.0, 2000.0, "%.0f");

    ImGui::PushTextWrapPos(0.f);   // prelomi tekst na sirinu panela
    ImGui::TextDisabled("Weight changes make the fitness curve discontinuous.");
    ImGui::PopTextWrapPos();
}

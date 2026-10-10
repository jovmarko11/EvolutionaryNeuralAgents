//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#include "StatisticsPanel.h"

#include <algorithm>
#include <cstdio>
#include <span>
#include <imgui.h>
#include <implot.h>

namespace {
    using History = std::span<const GenerationStatistics>;

    // Semanticke boje (iste kao u ostatku GUI-ja).
    constexpr ImVec4 bestColor    {1.00f, 0.82f, 0.29f, 1.f};   // zlatna
    constexpr ImVec4 meanColor    {0.31f, 0.62f, 1.00f, 1.f};   // plava
    constexpr ImVec4 medianColor  {0.44f, 0.84f, 0.79f, 1.f};   // tirkizna

    constexpr ImVec4 reachedColor {0.24f, 0.86f, 0.59f, 1.f};   // zelena
    constexpr ImVec4 crashedColor {1.00f, 0.36f, 0.42f, 1.f};   // crvena
    constexpr ImVec4 timeoutColor {0.95f, 0.72f, 0.29f, 1.f};   // zuta

    // Clanovi fitnessa najbolje jedinke (goal = zelena kao reached, collision = crvena kao crashed).
    constexpr ImVec4 progressColor {0.31f, 0.62f, 1.00f, 1.f};  // plava
    constexpr ImVec4 survivalColor {0.65f, 0.55f, 0.98f, 1.f};  // ljubicasta
    constexpr ImVec4 goalColor     {0.24f, 0.86f, 0.59f, 1.f};  // zelena
    constexpr ImVec4 speedColor    {0.90f, 0.45f, 0.78f, 1.f};  // roze
    constexpr ImVec4 collisionColor{1.00f, 0.36f, 0.42f, 1.f};  // crvena

    constexpr ImVec4 neutralColor  {0.55f, 0.58f, 0.65f, 1.f};  // siva (bez promene / bez podataka)

    // Trend na karticama: poredi se sa generacijom koja je ovoliko ranije (ili sa prvom, ako ih je manje).
    constexpr std::size_t trendWindow = 10;

    // Crta jednu seriju direktno iz niza GenerationStatistics, bez kopiranja:
    // Y je izabrano polje svakog elementa (stride = velicina elementa), X je indeks,
    // a indeks u istoriji je jednak broju generacije.
    template <typename T>
    void plotSeries(const char* label,
                    History history,
                    T GenerationStatistics::* field,
                    const ImVec4& color,
                    float weight) {
        ImPlot::PlotLine(label,
                         &(history.front().*field),
                         static_cast<int>(history.size()),
                         1.0, 0.0,
                         {ImPlotProp_LineColor, color,
                          ImPlotProp_LineWeight, weight,
                          ImPlotProp_Stride, static_cast<int>(sizeof(GenerationStatistics))});
    }

    // Kumulativni udeo ishoda (u procentima) zakljucno sa slojem Layer:
    // 0 = nista, 1 = reached, 2 = reached + crashed, 3 = sva tri (uvek 100%).
    // Dva susedna sloja zajedno ogranicavaju jednu traku slozene povrsine.
    template <int Layer>
    ImPlotPoint cumulativeShare(int idx, void* userData) {
        const GenerationStatistics& g = (*static_cast<const History*>(userData))[static_cast<std::size_t>(idx)];

        const double total = static_cast<double>(g.reach + g.crash + g.tout);
        double sum = 0.0;
        if (Layer >= 1) sum += g.reach;
        if (Layer >= 2) sum += g.crash;
        if (Layer >= 3) sum += g.tout;

        return ImPlotPoint(static_cast<double>(idx), total > 0.0 ? 100.0 * sum / total : 0.0);
    }

    // Pozitivni clanovi fitnessa najbolje jedinke slazu se iznad nule, odozdo naviše:
    // progress, survival, goal, speed. Layer = koliko prvih clanova je ukljuceno (0..4).
    template <int Layer>
    ImPlotPoint stackedPositive(int idx, void* userData) {
        const FitnessBreakdown& b = (*static_cast<const History*>(userData))[static_cast<std::size_t>(idx)].bestBreakdown;

        double sum = 0.0;
        if (Layer >= 1) sum += std::max(0.0, b.progress);
        if (Layer >= 2) sum += std::max(0.0, b.survival);
        if (Layer >= 3) sum += std::max(0.0, b.goal);
        if (Layer >= 4) sum += std::max(0.0, b.speed);

        return ImPlotPoint(static_cast<double>(idx), sum);
    }

    // Kazna za sudar je negativna pa se crta ispod nule (od 0 do -|collision|).
    ImPlotPoint zeroLine(int idx, void*) {
        return ImPlotPoint(static_cast<double>(idx), 0.0);
    }

    ImPlotPoint collisionLine(int idx, void* userData) {
        const FitnessBreakdown& b = (*static_cast<const History*>(userData))[static_cast<std::size_t>(idx)].bestBreakdown;
        return ImPlotPoint(static_cast<double>(idx), std::min(0.0, b.collision));
    }

    // Procenat agenata koji su stigli do cilja u toj generaciji.
    double reachRate(const GenerationStatistics& g) {
        const double total = static_cast<double>(g.reach + g.crash + g.tout);
        return total > 0.0 ? 100.0 * g.reach / total : 0.0;
    }

    // Jedna KPI kartica: mala oznaka, krupan broj i red sa trendom.
    void kpiCard(const char* label, const char* value, const ImVec4& valueColor,
                 const char* detail, const ImVec4& detailColor, float width) {
        ImGui::BeginChild(label, {width, 80}, ImGuiChildFlags_Borders,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::TextDisabled("%s", label);
        ImGui::SetWindowFontScale(1.7f);
        ImGui::TextColored(valueColor, "%s", value);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::TextColored(detailColor, "%s", detail);
        ImGui::EndChild();
    }

    // Boja trenda: zelena ako raste, crvena ako pada, siva ako je isto. (Za sve tri kartice je vise = bolje.)
    const ImVec4& trendColor(double delta) {
        if (delta > 0.0) return reachedColor;
        if (delta < 0.0) return crashedColor;
        return neutralColor;
    }

    void kpiRow(History history) {
        const GenerationStatistics& now = history.back();

        const std::size_t lookback = std::min(trendWindow, history.size() - 1);
        const GenerationStatistics& before = history[history.size() - 1 - lookback];

        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        const float width = (ImGui::GetContentRegionAvail().x - 2.f * spacing) / 3.f;

        char value[32], detail[48], label[48];

        // Prva generacija nema sa cim da se poredi.
        auto formatTrend = [&](double delta, const char* unit) {
            if (lookback == 0) std::snprintf(detail, sizeof(detail), "first generation");
            else std::snprintf(detail, sizeof(detail), "%+.0f%s vs %zu gen ago", delta, unit, lookback);
        };
        auto color = [&](double delta) -> const ImVec4& { return lookback == 0 ? neutralColor : trendColor(delta); };

        // 1) najbolji fitness
        double delta = now.bestFitness - before.bestFitness;
        std::snprintf(value, sizeof(value), "%.0f", now.bestFitness);
        formatTrend(delta, "");
        kpiCard("Best fitness", value, bestColor, detail, color(delta), width);
        ImGui::SameLine();

        // 2) procenat agenata koji stizu do cilja
        delta = reachRate(now) - reachRate(before);
        std::snprintf(value, sizeof(value), "%.0f%%", reachRate(now));
        formatTrend(delta, " pts");
        kpiCard("Reach rate", value, reachedColor, detail, color(delta), width);
        ImGui::SameLine();

        // 3) prosecan fitness (uz standardnu devijaciju u oznaci)
        delta = now.meanFitness - before.meanFitness;
        std::snprintf(label, sizeof(label), "Mean (std %.0f)", now.stdevFitness);
        std::snprintf(value, sizeof(value), "%.0f", now.meanFitness);
        formatTrend(delta, "");
        kpiCard(label, value, meanColor, detail, color(delta), width);
    }

    void plotFitness(History history, ImPlotAxisFlags fit, float height) {
        if (!ImPlot::BeginPlot("Fitness per generation", {-1, height})) return;

        ImPlot::SetupAxes("Generation", "Fitness", fit, fit);

        plotSeries("median", history, &GenerationStatistics::medianFitness, medianColor, 1.2f);
        plotSeries("mean",   history, &GenerationStatistics::meanFitness,   meanColor,   1.8f);
        plotSeries("best",   history, &GenerationStatistics::bestFitness,   bestColor,   2.2f);

        ImPlot::EndPlot();
    }

    void plotOutcomes(History history, ImPlotAxisFlags fit, float height) {
        if (!ImPlot::BeginPlot("Outcomes (% of population)", {-1, height})) return;

        // Y je uvek 0-100%, zakljucan; X prati podatke kao kod fitnessa.
        ImPlot::SetupAxes("Generation", "%", fit, ImPlotAxisFlags_Lock);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 100.0, ImPlotCond_Always);

        History view = history;                       // getteri dobijaju void*, pa nije const
        const int count = static_cast<int>(view.size());
        auto band = [&](const char* label, ImPlotGetter lower, ImPlotGetter upper, const ImVec4& color) {
            ImPlot::PlotShadedG(label, lower, &view, upper, &view, count,
                                {ImPlotProp_FillColor, color, ImPlotProp_FillAlpha, 0.85f});
        };

        band("reached", cumulativeShare<0>, cumulativeShare<1>, reachedColor);
        band("crashed", cumulativeShare<1>, cumulativeShare<2>, crashedColor);
        band("timeout", cumulativeShare<2>, cumulativeShare<3>, timeoutColor);

        ImPlot::EndPlot();
    }

    void plotBreakdown(History history, ImPlotAxisFlags fit, float height) {
        if (!ImPlot::BeginPlot("Best individual: fitness breakdown", {-1, height})) return;

        ImPlot::SetupAxes("Generation", "Fitness", fit, fit);

        History view = history;                       // getteri dobijaju void*, pa nije const
        const int count = static_cast<int>(view.size());
        auto band = [&](const char* label, ImPlotGetter lower, ImPlotGetter upper, const ImVec4& color) {
            ImPlot::PlotShadedG(label, lower, &view, upper, &view, count,
                                {ImPlotProp_FillColor, color, ImPlotProp_FillAlpha, 0.85f});
        };

        band("progress",  stackedPositive<0>, stackedPositive<1>, progressColor);
        band("survival",  stackedPositive<1>, stackedPositive<2>, survivalColor);
        band("goal",      stackedPositive<2>, stackedPositive<3>, goalColor);
        band("speed",     stackedPositive<3>, stackedPositive<4>, speedColor);
        band("collision", zeroLine,           collisionLine,      collisionColor);

        ImPlot::EndPlot();
    }
}

void StatisticsPanel::drawContent(const UIData& data, UIActions&) {
    const History history = data.statistics.history;

    ImGui::Checkbox("Auto-fit", &autoFit);
    ImGui::SameLine();
    ImGui::TextDisabled("%zu generations", history.size());

    if (history.empty()) {
        ImGui::TextDisabled("Waiting for the first generation to finish...");
        return;
    }

    kpiRow(history);

    const ImPlotAxisFlags fit = autoFit ? ImPlotAxisFlags_AutoFit : ImPlotAxisFlags_None;

    // Tri grafika dele sav preostali prostor panela (fitness je najbitniji pa je najvisi).
    const float gaps = 2.f * ImGui::GetStyle().ItemSpacing.y;
    const float available = std::max(0.f, ImGui::GetContentRegionAvail().y - gaps);
    constexpr float minHeight = 90.f;

    plotFitness  (history, fit, std::max(minHeight, available * 0.38f));
    plotOutcomes (history, fit, std::max(minHeight, available * 0.28f));
    plotBreakdown(history, fit, std::max(minHeight, available * 0.34f));
}

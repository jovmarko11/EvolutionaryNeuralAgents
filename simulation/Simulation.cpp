//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#include "Simulation.h"

#include <algorithm>
#include <cstdio>

Simulation::Simulation(World& world, Population& population, int maxSteps)
    : world(world),
      population(population),
      renderer(world, population),
      maxSteps(maxSteps),
      generation(0) {}


void Simulation::startGeneration() {
    if (!episodes.empty()) {
        generation++;
    }

    episodes.clear();
    episodes.reserve(population.getAgents().size());

    for (Agent& agent : population.getAgents()) {
        episodes.emplace_back(world, agent, maxSteps);
    }
}


void Simulation::update() {
    if (generationFinished()) {
        return;
    }

    for (Episode& episode : episodes) {
        episode.step();
    }

}


bool Simulation::generationFinished() const {
    return std::all_of(
        episodes.begin(), episodes.end(),
        [](const Episode& episode) { return episode.isFinished(); }
    );
}


void Simulation::render(sf::RenderWindow& window) const {
    renderer.render(window);
}

void Simulation::recordGenerationStats() {
    if (episodes.empty()) return;

    std::vector<EpisodeResult> results;
    results.reserve(episodes.size());

    for (const Episode& episode : episodes) {
        results.push_back(episode.getResult());
    }

    statistics.processGeneration(generation, results);
}

void Simulation::printGenerationStats() const {
    if (statistics.getHistory().empty()) return;

    const GenerationStatistics& gs = statistics.getLatest();

    std::printf("%4d | %8.1f %7.1f %7.1f %7.1f %7.1f | %5d %5d %5d | %7.2f %7.1f | %7.1f %5.1f %6.1f %6.1f\n",
        gs.generation,
        gs.bestFitness, gs.meanFitness, gs.medianFitness, gs.worstFitness, gs.stdevFitness,
        gs.reach, gs.crash, gs.tout,
        gs.maxProgress, gs.averageSteps,
        gs.bestBreakdown.progress, gs.bestBreakdown.survival, gs.bestBreakdown.goal, gs.bestBreakdown.collision);
}

void Simulation::printGenerationStatsHeader() {
    std::printf("%4s | %8s %7s %7s %7s %7s | %5s %5s %5s | %7s %7s | %7s %5s %6s %6s\n",
        "gen", "best", "mean", "median", "worst", "stdev",
        "reach", "crash", "tout", "maxProg", "avgStep", "prog", "surv", "goal", "crash");
}

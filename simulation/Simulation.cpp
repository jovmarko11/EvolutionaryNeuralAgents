//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#include "Simulation.h"
#include "../fitness/Fitness.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ostream>

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

void Simulation::printGenerationStats() const {
    if (episodes.empty()) return;

    std::vector<double> values;
    values.reserve(episodes.size());

    const EpisodeResult* best = nullptr;
    double bestFitness = -std::numeric_limits<double>::infinity();
    int reached = 0, crashed = 0, timedOut = 0;
    double sumSteps = 0.0, maxProgress = 0.0;

    for (const Episode& episode : episodes) {
        const EpisodeResult& r = episode.getResult();
        const double f = fitness(r);

        values.push_back(f);
        if (f > bestFitness) { bestFitness = f; best = &r; }

        reached  += r.reachedTarget;
        crashed  += r.crashed;
        timedOut += r.timeOut;

        sumSteps += r.steps;
        maxProgress = std::max(maxProgress, r.progress());
    }

    std::sort(values.begin(), values.end());

    const std::size_t n = values.size();
    const double mean = std::accumulate(values.begin(), values.end(), 0.0) / n;
    const double median = (n % 2) ? values[n/2] : 0.5 * (values[n/2 - 1] + values[n/2]);

    double variance = 0.0;
    for (const double v : values) variance += (v - mean) * (v - mean);

    const FitnessBreakdown b = fitnessBreakdown(*best);

    std::printf("%4d | %8.1f %7.1f %7.1f %7.1f %7.1f | %5d %5d %5d | %7.2f %7.1f | %7.1f %5.1f %6.1f %6.1f\n",
        generation,
        values.back(), mean, median, values.front(), std::sqrt(variance / n),
        reached, crashed, timedOut,
        maxProgress, sumSteps / n,
        b.progress, b.survival, b.goal, b.collision);
}

void Simulation::printGenerationStatsHeader() {
    std::printf("%4s | %8s %7s %7s %7s %7s | %5s %5s %5s | %7s %7s | %7s %5s %6s %6s\n",
        "gen", "best", "mean", "median", "worst", "stdev",
        "reach", "crash", "tout", "maxProg", "avgStep", "prog", "surv", "goal", "crash");
}


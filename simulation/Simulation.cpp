//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#include "Simulation.h"

#include <algorithm>

Simulation::Simulation(World& world, Population& population, int maxSteps, const FitnessWeights& weights)
    : world(world),
      population(population),
      renderer(world, population),
      maxSteps(maxSteps),
      generation(0),
      statistics(weights),
      weights(weights) {}


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


void Simulation::render(sf::RenderWindow& window, const sf::FloatRect& area) const {
    updateBestAgentHighlight();
    renderer.render(window, area);
}

void Simulation::recordGenerationStats(const GAParams& gaParams) {
    if (episodes.empty()) return;

    std::vector<EpisodeResult> results;
    results.reserve(episodes.size());

    for (const Episode& episode : episodes) {
        results.push_back(episode.getResult());
    }

    statistics.processGeneration(generation, results, gaParams);
}

std::size_t Simulation::bestAgentIndex() const {
    // called when episodes vector is not empty
    std::size_t bestIndex = 0;
    double bestFitness = fitness(episodes[0].getResult(), weights);

    for (std::size_t i = 0; i < episodes.size(); ++i) {
        double candidateFitness = fitness(episodes[i].getResult(), weights);
        if (candidateFitness > bestFitness) {
            bestFitness = candidateFitness;
            bestIndex = i;
        }
    }
    return bestIndex;
}

void Simulation::updateBestAgentHighlight() const {
    if (episodes.empty()) return;

    for (Agent& agent : population.getAgents()) {
        agent.setHighlighted(false);
    }

    std::size_t bestIndex = bestAgentIndex();

    population.getAgents()[bestIndex].setHighlighted(true);
}

void Simulation::setWeights(const FitnessWeights& w) {
    weights = w;
    statistics.setWeights(w);
}

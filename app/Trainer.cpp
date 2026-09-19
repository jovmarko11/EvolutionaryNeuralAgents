//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "Trainer.h"
#include <cassert>

Trainer::Trainer(Simulation& simulation, GeneticAlgorithm& ga)
    :   simulation(simulation),
        ga(ga) {}

void Trainer::step(int steps, Population& population) {
    advanceGeneration(population);

    for (int i = 0; i < steps; i++) {
        simulation.update();
    }
}

void Trainer::advanceGeneration(Population& population) {
    if (!simulation.generationFinished()) return;

    simulation.printGenerationStats();

    const std::vector<Individual> current = collect(population);
    const std::vector<std::vector<double>> next = ga.nextGeneration(current);

    assert(next.size() == current.size());

    apply(population, next);

    simulation.startGeneration();
}

void Trainer::apply(Population& population, const std::vector<std::vector<double>>& genomes) {
    auto& agents = population.getAgents();
    assert(agents.size() == genomes.size());

    for (std::size_t i = 0; i < agents.size(); ++i) {
        agents[i].getBrain().setParameters(genomes[i]);
    }
}

std::vector<Individual> Trainer::collect(const Population& population) const {
    const auto& agents = population.getAgents();
    assert(agents.size() == simulation.getEpisodes().size());

    std::vector<Individual> current;
    current.reserve(agents.size());

    for (std::size_t i = 0; i < agents.size(); ++i) {
        current.push_back({ agents[i].getBrain().getParameters(), fitness(simulation.getEpisodes()[i].getResult()) });
    }

    return current;
}

//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "Trainer.h"
#include <algorithm>
#include <cassert>

#include "learning/statistics/ConsoleReporter.h"

Trainer::Trainer(Simulation& simulation, GeneticAlgorithm& ga, const FitnessWeights& weights)
    :   simulation(simulation),
        ga(ga),
        weights(weights) {}

GAParams Trainer::sanitize(GAParams p, std::size_t populationSize) {
    p.elitismRatio = std::clamp(p.elitismRatio, 0.0, 1.0);
    p.mutationRatio = std::clamp(p.mutationRatio, 0.0, 1.0);
    p.mutationStrength = std::max(p.mutationStrength, 0.0);
    p.tournamentSize = std::clamp(p.tournamentSize, 1, static_cast<int>(std::max<std::size_t>(populationSize, 1)));
    return p;
}

void Trainer::step(int steps, Population& population) {
    advanceGeneration(population);

    for (int i = 0; i < steps; i++) {
        simulation.update();
    }
}

void Trainer::advanceGeneration(Population& population) {
    if (!simulation.generationFinished()) return;

    // Snapshot parametara GA mora da se uzme PRE nego sto ih bilo ko menja
    // (npr. ParamController): red generacije g opisuje parametre kojima je
    // g proizvedena.
    simulation.recordGenerationStats(ga.getParams());

    const Statistics& stats = simulation.getStatistics();
    if (!stats.getHistory().empty()) {
        ConsoleReporter::printRow(stats.getLatest());
    }

    const std::vector<Individual> current = collect(population);

    // GA parametri: posle snimka statistike, pre selekcije -> vaze vec za sledecu generaciju.
    if (pending) ga.setParams(sanitize(pending->ga, current.size()));

    const std::vector<std::vector<double>> next = ga.nextGeneration(current);

    // Tezine fitnesa: tek posle collect() -> generacija g je ocenjena starim tezinama,
    // a g+1 ce biti ocenjena novim.
    if (pending) {
        weights = pending->fitness;
        simulation.setWeights(weights);
        pending.reset();
    }

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
        current.push_back({ agents[i].getBrain().getParameters(), fitness(simulation.getEpisodes()[i].getResult(), weights) });
    }

    return current;
}

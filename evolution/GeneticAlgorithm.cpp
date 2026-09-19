//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "GeneticAlgorithm.h"


GeneticAlgorithm::GeneticAlgorithm(double elitismRatio, int tournamentSize, double mutationRatio, double mutationStrength)
    :   elitismRatio(elitismRatio),
        mutation(mutationRatio, mutationStrength),
        selection(tournamentSize)  {}


std::vector<std::vector<double> > GeneticAlgorithm::nextGeneration(const std::vector<Individual> &current) const {
    std::vector<std::vector<double> > nextGen;
    nextGen.reserve(current.size());

    std::vector<Individual> sorted = current;
    std::sort(sorted.begin(), sorted.end(),
        [](const Individual& a, const Individual& b) { return a.fitness > b.fitness; });

    std::size_t eliteCount = std::round(current.size() * elitismRatio);

    for (std::size_t i = 0; i < eliteCount; i++) {
        nextGen.push_back(std::move(sorted[i].genome));
    }

    while (nextGen.size() < current.size()) {
        const Individual& a = selection.tournament(current);
        const Individual& b = selection.tournament(current);

        std::vector<double> child = Crossover::uniform(a.genome, b.genome);
        mutation.mutate(child);
        nextGen.push_back(std::move(child));
    }
    return nextGen;
}
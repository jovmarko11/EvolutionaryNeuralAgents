//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_TRAINER_H
#define EVOLUTIONARYNEURALAGENTS_TRAINER_H
#include "model/simulation/Simulation.h"
#include "learning/evolution/GeneticAlgorithm.h"
#include "learning/fitness/Fitness.h"
#include "app/config/TrainingParams.h"
#include <optional>


class Trainer {

    Simulation& simulation;
    GeneticAlgorithm& ga;
    FitnessWeights weights;

    // Zahtev za promenu parametara; primenjuje se na granici generacije.
    std::optional<TrainingParams> pending;

    std::vector<Individual> collect(const Population& population) const;
    void apply(Population& population, const std::vector<std::vector<double>>& genomes);
    void advanceGeneration(Population& population);
    static GAParams sanitize(GAParams p, std::size_t populationSize);

public:
    Trainer(Simulation& simulation, GeneticAlgorithm& ga, const FitnessWeights& weights = FitnessWeights{});


    void step(int steps, Population& population);

    void requestParams(const TrainingParams& params) { pending = params; }
    bool hasPendingParams() const { return pending.has_value(); }
    TrainingParams currentParams() const { return {ga.getParams(), weights}; }
};


#endif //EVOLUTIONARYNEURALAGENTS_TRAINER_H

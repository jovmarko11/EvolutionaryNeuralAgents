//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_TRAINER_H
#define EVOLUTIONARYNEURALAGENTS_TRAINER_H
#include "../simulation/Simulation.h"
#include "../evolution/GeneticAlgorithm.h"
#include "../fitness/Fitness.h"


class Trainer {

    Simulation& simulation;
    GeneticAlgorithm& ga;

    std::vector<Individual> collect(const Population& population) const;
    void apply(Population& population, const std::vector<std::vector<double>>& genomes);
    void advanceGeneration(Population& population);

public:
    Trainer(Simulation& simulation, GeneticAlgorithm& ga);


    void step(int steps, Population& population);
};


#endif //EVOLUTIONARYNEURALAGENTS_TRAINER_H

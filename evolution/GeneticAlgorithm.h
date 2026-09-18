//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
#define EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
#include "Crossover.h"
#include "Mutation.h"
#include "Selection.h"
#include "../simulation/Population.h"

struct Individual {
    std::vector<double> genome;
    double fitness = 0.0;
};

class GeneticAlgorithm {
    double elitismRatio, mutationRatio, mutationStrength;

public:
    GeneticAlgorithm(double elitismRatio = 0.08, double mutationRatio = 0.10, double mutationStrength = 0.10);

    std::vector<std::vector<double>> nextGeneration(const std::vector<Individual>& current) const;

};


#endif //EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
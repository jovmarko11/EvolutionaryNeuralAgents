//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
#define EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
#include "Crossover.h"
#include "Individual.h"
#include "Mutation.h"
#include "Selection.h"
#include "../simulation/Population.h"
#include "../configuration/Params.h"


class GeneticAlgorithm {
    double elitismRatio;

    Mutation mutation;
    Selection selection;

public:
    explicit GeneticAlgorithm(double elitismRatio = Params::elitismRatio,
                    int tournamentSize = Params::tournamentSize,
                    double mutationRatio = Params::mutationRatio,
                    double mutationStrength = Params::mutationStrength);

    std::vector<std::vector<double>> nextGeneration(const std::vector<Individual>& current) const;

};


#endif //EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
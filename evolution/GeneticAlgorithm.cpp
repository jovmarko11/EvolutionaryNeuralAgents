//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "GeneticAlgorithm.h"


GeneticAlgorithm::GeneticAlgorithm(double elitismRatio, double mutationRatio, double mutationStrength)
    :   elitismRatio(elitismRatio),
        mutationRatio(mutationRatio),
        mutationStrength(mutationStrength)  {}


std::vector<std::vector<double> > GeneticAlgorithm::nextGeneration(const std::vector<Individual> &current) const {

    std::vector<std::vector<double> > nextGen;
    return nextGen;
}
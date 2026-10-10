//
// Created by Marko Jovanovic on 29. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_EXPERIMENTCONFIG_H
#define EVOLUTIONARYNEURALAGENTS_EXPERIMENTCONFIG_H
#include "Params.h"
#include "../fitness/Fitness.h"
#include <nlohmann/json.hpp>

struct ExperimentConfig {
    unsigned int seed = Params::randomSeed;
    int numAgents = Params::numAgents;
    int maxSteps = Params::maxSteps;
    int generations = 500;

    double elitismRatio = Params::elitismRatio;
    double mutationRatio = Params::mutationRatio;
    double mutationStrength = Params::mutationStrength;
    int tournamentSize = Params::tournamentSize;

    double fitnessProgress = FitnessWeights{}.progress;
    double fitnessSurvival = FitnessWeights{}.survival;
    double fitnessGoal = FitnessWeights{}.goal;
    double fitnessCollision = FitnessWeights{}.collision;
    double fitnessSpeed = FitnessWeights{}.speed;

    bool operator==(const ExperimentConfig &other) const = default;
};

void to_json(nlohmann::json& j, const ExperimentConfig& c);
void from_json(const nlohmann::json& j, ExperimentConfig& c);

#endif //EVOLUTIONARYNEURALAGENTS_EXPERIMENTCONFIG_H
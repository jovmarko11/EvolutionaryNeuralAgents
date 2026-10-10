//
// Created by Marko Jovanovic on 30. 9. 2026..
//
#include "ExperimentConfig.h"

void to_json(nlohmann::json& j, const ExperimentConfig& c) {
    j = nlohmann::json{
            {"seed", c.seed},
            {"numAgents", c.numAgents},
            {"maxSteps", c.maxSteps},
            {"generations", c.generations},
            {"elitismRatio", c.elitismRatio},
            {"mutationRatio", c.mutationRatio},
            {"mutationStrength", c.mutationStrength},
            {"tournamentSize", c.tournamentSize},
            {"fitnessProgress", c.fitnessProgress},
            {"fitnessSurvival", c.fitnessSurvival},
            {"fitnessGoal", c.fitnessGoal},
            {"fitnessCollision", c.fitnessCollision},
            {"fitnessSpeed", c.fitnessSpeed}
    };
}

void from_json(const nlohmann::json& j, ExperimentConfig& c) {
    j.at("seed").get_to(c.seed);
    j.at("numAgents").get_to(c.numAgents);
    j.at("maxSteps").get_to(c.maxSteps);
    j.at("generations").get_to(c.generations);
    j.at("elitismRatio").get_to(c.elitismRatio);
    j.at("mutationRatio").get_to(c.mutationRatio);
    j.at("mutationStrength").get_to(c.mutationStrength);
    j.at("tournamentSize").get_to(c.tournamentSize);
    j.at("fitnessProgress").get_to(c.fitnessProgress);
    j.at("fitnessSurvival").get_to(c.fitnessSurvival);
    j.at("fitnessGoal").get_to(c.fitnessGoal);
    j.at("fitnessCollision").get_to(c.fitnessCollision);
    j.at("fitnessSpeed").get_to(c.fitnessSpeed);
}
//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PARAMS_H
#define EVOLUTIONARYNEURALAGENTS_PARAMS_H
#include <numbers>
#include <vector>

#include "../rendering/Color.h"

namespace Params {

    // Simulation
    inline constexpr int visualizeEveryN = 1;
    inline constexpr int visualizeFromGeneration = 0;
    inline constexpr double hiddenSpeedMultiplier = 1;

    // Svet
    inline constexpr double worldLimitX = 20.0;
    inline constexpr double worldLimitY = 30.0;

    // Populacija
    inline constexpr int numAgents = 100;
    inline constexpr int maxSteps = 1000;   // duzina epizode
    inline constexpr unsigned int randomSeed = 42;

    // Start agenata. Svi krecu odavde, da bi fitness bio uporediv:
    // razlika u rezultatu sme da dolazi samo od mreze, ne od pozicije.
    inline constexpr double spawnX = -15.0;
    inline constexpr double spawnY = -25.0;
    inline constexpr double spawnHeading = 0.0;    // rad; 0 = +x
    inline constexpr double spawnClearance = 0.5;  // minimalni razmak od prepreke i granice

    // Agent
    inline constexpr int numOfSensors = 7;
    inline constexpr double fieldOfView = std::numbers::pi;
    inline constexpr float agentRadius = 0.3f;

    inline constexpr double maxCollisionStep = 0.1;

    // RaySensor
    inline constexpr double sensorMaxDistance = 6.0;
    inline constexpr double sensorStepSize = 0.2;

    // Boje
    inline constexpr Color AgentColor{220, 40, 40, 255};
    inline constexpr Color ObstacleColor{255, 255, 255, 255};
    inline constexpr Color TargetColor{40, 90, 220, 255};

    // GeneticAlgorithm
    inline constexpr double elitismRatio = 0.08;
    inline constexpr double mutationRatio = 0.20;
    inline constexpr double mutationStrength = 0.20;
    inline constexpr int tournamentSize = 5;

    // Mreza

    inline constexpr int inputsPerSensor = 1;  // proximity = 1 - distanca
    inline constexpr int extraInputs = 4;      // sin i cos ugla ka cilju, distanceToGoal, speed
    inline constexpr int inputSize = numOfSensors * inputsPerSensor + extraInputs;
    inline constexpr int outputSize = 2;       // turn, speed

    inline constexpr double maxSpeed    = 10.0;              // jed/s
    inline constexpr double maxTurnRate = std::numbers::pi;  // rad/s
    inline constexpr double timeStep    = 0.02;              // s/korak

    inline const std::vector<std::size_t> networkLayerSizes = {
        static_cast<std::size_t>(inputSize),
        10,                                      // skriveni sloj
        static_cast<std::size_t>(outputSize)
    };


    // window
    inline constexpr unsigned windowWidth = 800;
    inline constexpr unsigned windowHeight = 600;
    inline constexpr unsigned framerateLimit = 120;
}

#endif //EVOLUTIONARYNEURALAGENTS_PARAMS_H

//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PARAMS_H
#define EVOLUTIONARYNEURALAGENTS_PARAMS_H
#include <numbers>
#include <vector>

#include "../rendering/Color.h"

namespace Params {

    // Svet
    inline constexpr double worldLimitX = 20.0;
    inline constexpr double worldLimitY = 30.0;

    // Populacija
    inline constexpr int numAgents = 6;
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

    // Mreza
    // Ulaz mreze. Po senzoru samo blizina; status reja se ne salje mrezi
    // (jedini bit koji nije rekonstruktivan iz blizine razlikovao bi zid od
    // granice sveta, a oba se izbegavaju isto).
    inline constexpr int inputsPerSensor = 1;  // proximity = 1 - distanca
    inline constexpr int extraInputs = 4;      // sin i cos ugla ka cilju, distanceToGoal, speed
    inline constexpr int inputSize = numOfSensors * inputsPerSensor + extraInputs;
    inline constexpr int outputSize = 2;       // turn, speed

    // Duzine su u jedinicama sveta, vreme u sekundama.
    // maxSpeed i maxTurnRate su BRZINE po sekundi — mnoze se sa timeStep.
    inline constexpr double maxSpeed    = 10.0;              // jed/s
    inline constexpr double maxTurnRate = std::numbers::pi;  // rad/s
    inline constexpr double timeStep    = 0.02;              // s/korak

    // Izvedeno (proveri kad menjas gornje tri vrednosti):
    //   pomeraj po koraku    = maxSpeed * timeStep        = 0.20 jed
    //   rotacija po koraku   = maxTurnRate * timeStep     = 3.6 st
    //   poluprecnik zaokreta = maxSpeed / maxTurnRate     = 3.18 jed
    //   trajanje epizode     = maxSteps * timeStep        = 20 s simuliranog vremena
    //   (realno vreme zavisi od FPS-a — vidi app/FixedTimestep)

    inline const std::vector<std::size_t> networkLayerSizes = {
        static_cast<std::size_t>(inputSize),
        10,                                      // skriveni sloj
        static_cast<std::size_t>(outputSize)
    };

}

#endif //EVOLUTIONARYNEURALAGENTS_PARAMS_H

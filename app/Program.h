//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PROGRAM_H
#define EVOLUTIONARYNEURALAGENTS_PROGRAM_H
#include <SFML/Graphics/RenderWindow.hpp>

#include "FixedTimestep.h"
#include "Trainer.h"
#include "../evolution/GeneticAlgorithm.h"
#include "../simulation/Simulation.h"
#include "../math/Random.h"
#include "../simulation/SceneBuilder.h"

class Program {

    sf::RenderWindow window;
    FixedTimestep timeStep;

    World world;
    Population population;

    GeneticAlgorithm ga;
    Simulation simulation;

    Trainer trainer;

    bool visualizationEnabled = true;

    void handleEvents();
    void drawFrame();

    bool shouldVisualize() const;
    void updateVisualizationPolicy();

public:
    Program();

    void run();
};


#endif //EVOLUTIONARYNEURALAGENTS_PROGRAM_H
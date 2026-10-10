//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PROGRAM_H
#define EVOLUTIONARYNEURALAGENTS_PROGRAM_H
#include <SFML/Graphics/RenderWindow.hpp>

#include <chrono>

#include "FixedTimestep.h"
#include "Trainer.h"
#include "../evolution/GeneticAlgorithm.h"
#include "../simulation/Simulation.h"
#include "../gui/UILayer.h"
#include "../gui/UIData.h"
#include "../simulation/SceneBuilder.h"

class Program {

    sf::RenderWindow window;
    UILayer ui;
    FixedTimestep timeStep;

    World world;
    Population population;

    GeneticAlgorithm ga;
    Simulation simulation;

    Trainer trainer;

    double visibleSpeed = Params::visibleSpeedMultiplier;
    double hiddenSpeed = Params::hiddenSpeedMultiplier;

    bool visualizationEnabled = true;

    // Merenje vremena (bez pauza) za status traku.
    int trackedGeneration = 0;
    double generationSeconds = 0.0;      // akumulirano za generaciju u toku
    double lastGenerationSeconds = 0.0;
    double trainingSeconds = 0.0;

    void trackTime(double frameSeconds);

    void handleEvents();
    void drawFrame();

    TransportInfo makeTransportInfo() const;
    UIData makeUIData() const;
    void applyActions(const UIActions& actions);

    bool shouldVisualize() const;
    void updateVisualizationPolicy();

public:
    Program();

    void run();

    void setVisibleSpeed(double speed);
    void setHiddenSpeed(double speed);
    double getVisibleSpeed() const;
    double getHiddenSpeed() const;
};



#endif //EVOLUTIONARYNEURALAGENTS_PROGRAM_H
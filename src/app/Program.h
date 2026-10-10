//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PROGRAM_H
#define EVOLUTIONARYNEURALAGENTS_PROGRAM_H
#include <SFML/Graphics/RenderWindow.hpp>

#include <chrono>

#include "FixedTimestep.h"
#include "Trainer.h"
#include "learning/evolution/GeneticAlgorithm.h"
#include "model/simulation/Simulation.h"
#include "ui/gui/UILayer.h"
#include "ui/gui/UIData.h"
#include "model/simulation/SceneBuilder.h"

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
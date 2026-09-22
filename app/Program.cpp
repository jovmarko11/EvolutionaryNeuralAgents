//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "Program.h"

Program::Program()
:   world(SceneBuilder::buildWorld()),
    population(SceneBuilder::buildPopulation(world)),
    simulation(world, population),
    trainer(simulation, ga) {
    Random::seed(Params::randomSeed);

    window = sf::RenderWindow(sf::VideoMode({Params::windowWidth, Params::windowHeight}), "Evolutionary Neural Agents");
    window.setFramerateLimit(Params::framerateLimit);

    simulation.startGeneration();

    Simulation::printGenerationStatsHeader();
}

void Program::run() {
    while (window.isOpen()) {
        handleEvents();
        updateVisualizationPolicy();

        trainer.step(timeStep.beginFrame(), population);

        if (visualizationEnabled) drawFrame();
    }
}


void Program::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Space) {
                timeStep.togglePause();
            }
            else if (key->code == sf::Keyboard::Key::Right) {
                timeStep.requestSingleStep();
            }
        }
    }
}

void Program::drawFrame() {
    window.clear();
    simulation.render(window);
    window.display();
}

bool Program::shouldVisualize() const {
    const int gen = simulation.getGeneration();

    if (gen < Params::visualizeFromGeneration) return false;

    return (gen - Params::visualizeFromGeneration) % Params::visualizeEveryN == 0;
}


void Program::updateVisualizationPolicy() {
    visualizationEnabled = shouldVisualize();

    if (!timeStep.isPaused()) {
        timeStep.setSpeedMultiplier(visualizationEnabled ? Params::visibleSpeedMultiplier : Params::hiddenSpeedMultiplier);
    }
}

void initWindow() {

}

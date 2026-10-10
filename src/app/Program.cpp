//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "Program.h"

#include <algorithm>

#include "learning/statistics/ConsoleReporter.h"
#include "core/math/Random.h"

Program::Program()
:   ui(window),
    world(SceneBuilder::buildWorld()),
    population(SceneBuilder::buildPopulation(world)),
    simulation(world, population),
    trainer(simulation, ga) {
    Random::seed(Params::randomSeed);

    window = sf::RenderWindow(sf::VideoMode({Params::windowWidth, Params::windowHeight}), "Evolutionary Neural Agents");
    window.setFramerateLimit(Params::framerateLimit);
    ui.init();

    simulation.startGeneration();
    trackedGeneration = simulation.getGeneration();

    ConsoleReporter::printHeader();
}


void Program::run() {
    auto previous = std::chrono::steady_clock::now();

    while (window.isOpen()) {
        const auto now = std::chrono::steady_clock::now();
        trackTime(std::chrono::duration<double>(now - previous).count());
        previous = now;

        handleEvents();

        updateVisualizationPolicy();

        applyActions(ui.beginFrame(makeUIData()));
        trainer.step(timeStep.beginFrame(), population);

        if (visualizationEnabled) drawFrame();
        else ui.skipFrame();
    }
}


void Program::trackTime(double frameSeconds) {
    if (!timeStep.isPaused()) {
        generationSeconds += frameSeconds;
        trainingSeconds += frameSeconds;
    }

    if (simulation.getGeneration() != trackedGeneration) {
        trackedGeneration = simulation.getGeneration();
        lastGenerationSeconds = generationSeconds;
        generationSeconds = 0.0;
    }
}

void Program::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        ui.handleEvent(*event);

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
    simulation.render(window, ui.simArea());
    ui.render();
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
        timeStep.setSpeedMultiplier(visualizationEnabled ? visibleSpeed : hiddenSpeed);
    }
}



void Program::setVisibleSpeed(double speed) {visibleSpeed = speed;}
void Program::setHiddenSpeed(double speed) {hiddenSpeed = speed;}
double Program::getVisibleSpeed() const {return visibleSpeed;}
double Program::getHiddenSpeed() const {return hiddenSpeed;}

TransportInfo Program::makeTransportInfo() const {
    TransportInfo t;
    t.generation = simulation.getGeneration();
    t.maxSteps = simulation.getMaxSteps();
    t.paused = timeStep.isPaused();
    t.visibleSpeed = visibleSpeed;
    for (const Episode& e : simulation.getEpisodes()) {
        const auto& r = e.getResult();
        t.step = std::max(t.step, r.steps);
        if (r.reachedTarget) t.reached++;
        else if (r.crashed)  t.crashed++;
        else if (r.timeOut)  t.timeout++;
        else                 t.alive++;
    }
    return t;
}

UIData Program::makeUIData() const {
    UIData data;
    data.transport = makeTransportInfo();
    data.statistics.history = simulation.getStatistics().getHistory();
    data.parameters.applied = trainer.currentParams();
    data.parameters.pending = trainer.hasPendingParams();
    data.status.lastGenerationSeconds = lastGenerationSeconds;
    data.status.trainingSeconds = trainingSeconds;
    data.parameters.populationSize = static_cast<int>(population.getAgents().size());
    return data;
}

void Program::applyActions(const UIActions& actions) {
    if (actions.togglePause) timeStep.togglePause();
    if (actions.step) timeStep.requestSingleStep();
    if (actions.setVisibleSpeed) setVisibleSpeed(*actions.setVisibleSpeed);
    if (actions.applyTraining) trainer.requestParams(*actions.applyTraining);
}

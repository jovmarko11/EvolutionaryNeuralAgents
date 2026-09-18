//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_SIMULATION_H
#define EVOLUTIONARYNEURALAGENTS_SIMULATION_H

#include <vector>
#include "Episode.h"
#include "Population.h"
#include "../world/World.h"
#include "../configuration/Params.h"
#include "../rendering/Renderer.h"

namespace sf { class RenderWindow; }

class Simulation {
    World& world;
    Population& population;
    Renderer renderer;

    std::vector<Episode> episodes;
    int maxSteps;
    int generation;

public:
    Simulation(World& world, Population& population, int maxSteps = Params::maxSteps);

    void startGeneration();

    void update();

    void render(sf::RenderWindow& window) const;

    bool generationFinished() const;
    const std::vector<Episode>& getEpisodes() const;
    int getGeneration() const;
    int getMaxSteps() const;

    void printGenerationStats() const;
    static void printGenerationStatsHeader();
};

inline const std::vector<Episode>& Simulation::getEpisodes() const { return episodes; }
inline int Simulation::getGeneration() const { return generation; }
inline int Simulation::getMaxSteps() const { return maxSteps; }

#endif //EVOLUTIONARYNEURALAGENTS_SIMULATION_H

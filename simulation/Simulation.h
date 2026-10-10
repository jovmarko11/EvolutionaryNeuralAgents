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
#include "../evolution/GAParams.h"
#include "../fitness/Fitness.h"
#include "../rendering/Renderer.h"
#include "../statistics/Statistics.h"

namespace sf { class RenderWindow; }

class Simulation {
    World& world;
    Population& population;
    Renderer renderer;

    FitnessWeights weights;

    std::vector<Episode> episodes;
    int maxSteps;
    int generation;

    Statistics statistics;

public:
    Simulation(World& world, Population& population, int maxSteps = Params::maxSteps,
               const FitnessWeights& weights = FitnessWeights{});

    void startGeneration();

    void update();

    void render(sf::RenderWindow& window, const sf::FloatRect& area) const;

    bool generationFinished() const;
    const std::vector<Episode>& getEpisodes() const;
    int getGeneration() const;
    int getMaxSteps() const;

    std::size_t bestAgentIndex() const;

    void recordGenerationStats(const GAParams& gaParams);

    const Statistics& getStatistics() const;

    // Primenjuje se samo na granici generacije (poziva Trainer).
    void setWeights(const FitnessWeights& w);

    void updateBestAgentHighlight() const;
};

inline const std::vector<Episode>& Simulation::getEpisodes() const { return episodes; }
inline int Simulation::getGeneration() const { return generation; }
inline int Simulation::getMaxSteps() const { return maxSteps; }
inline const Statistics& Simulation::getStatistics() const { return statistics; }

#endif //EVOLUTIONARYNEURALAGENTS_SIMULATION_H

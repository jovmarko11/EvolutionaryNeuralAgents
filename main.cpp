#include <cassert>
#include <vector>

#include <SFML/Graphics.hpp>

#include "app/FixedTimestep.h"
#include "evolution/GeneticAlgorithm.h"
#include "fitness/Fitness.h"
#include "math/Random.h"
#include "simulation/SceneBuilder.h"
#include "simulation/Simulation.h"

namespace {

constexpr unsigned windowWidth = 800;
constexpr unsigned windowHeight = 600;
constexpr unsigned framerateLimit = 120;

// Razmak / Space -> pauza, strelica desno -> jedan korak dok je pauza.
void handleEvents(sf::RenderWindow& window, FixedTimestep& timestep) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Space) {
                timestep.togglePause();
            }
            else if (key->code == sf::Keyboard::Key::Right) {
                timestep.requestSingleStep();
            }
        }
    }
}


// Genom + fitness svakog agenta. Jedino mesto koje istovremeno zna
// za agente i za fitness formulu.
std::vector<Individual> collect(const Population& population,
                                const std::vector<Episode>& episodes) {
    const auto& agents = population.getAgents();
    assert(agents.size() == episodes.size());

    std::vector<Individual> current;
    current.reserve(agents.size());

    for (std::size_t i = 0; i < agents.size(); ++i) {
        current.push_back({ agents[i].getBrain().getParameters(),
                            fitness(episodes[i].getResult()) });
    }

    return current;
}


// Upisuje nove genome u postojece agente. Agenti se ne prave iznova —
// menja im se samo mozak.
void apply(Population& population, const std::vector<std::vector<double>>& genomes) {
    auto& agents = population.getAgents();
    assert(agents.size() == genomes.size());

    for (std::size_t i = 0; i < agents.size(); ++i) {
        agents[i].getBrain().setParameters(genomes[i]);
    }
}


void evolveIfGenerationFinished(Simulation& simulation,
                                Population& population,
                                const GeneticAlgorithm& ga) {
    if (!simulation.generationFinished()) {
        return;
    }

    simulation.printGenerationStats();

    const std::vector<Individual> current = collect(population, simulation.getEpisodes());
    const std::vector<std::vector<double>> next = ga.nextGeneration(current);

    assert(next.size() == current.size());

    apply(population, next);

    simulation.startGeneration();
}


// Korak vremena, nezavisno od generacija.
void advanceSimulation(Simulation& simulation, int steps) {
    for (int i = 0; i < steps; ++i) {
        simulation.update();
    }
}


void drawFrame(sf::RenderWindow& window, const Simulation& simulation) {
    window.clear();
    simulation.render(window);
    window.display();
}

} // namespace


int main() {
    // Seed pre nego sto se napravi ijedan agent — tezine se izvlace odavde.
    Random::seed(Params::randomSeed);

    World world = SceneBuilder::buildWorld();
    Population population = SceneBuilder::buildPopulation(world);

    GeneticAlgorithm ga;
    Simulation simulation(world, population);
    simulation.startGeneration();

    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}),
                            "Evolutionary Neural Agents");
    window.setFramerateLimit(framerateLimit);

    FixedTimestep timestep;

    Simulation::printGenerationStatsHeader();

    while (window.isOpen()) {
        handleEvents(window, timestep);

        evolveIfGenerationFinished(simulation, population, ga);
        advanceSimulation(simulation, timestep.beginFrame());

        drawFrame(window, simulation);
    }

    return 0;
}

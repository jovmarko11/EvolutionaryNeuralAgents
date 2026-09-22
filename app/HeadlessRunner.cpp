//
// Created by Marko Jovanovic on 22. 9. 2026..
//

#include "HeadlessRunner.h"

#include <iostream>

#include "../simulation/SceneBuilder.h"
#include "../configuration/Params.h"
#include "../math/Random.h"
#include <string>

#include "Trainer.h"

namespace {

struct HeadlessArgs {
    int generations = 500;
    unsigned int seed = Params::randomSeed;
    std::string output = "statistics.csv";
};


HeadlessArgs parseArgs(int argc, char** argv) {
    HeadlessArgs args;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--generations" && i + 1 < argc) {
            args.generations = std::stoi(argv[++i]);
        } else if (arg == "--seed" && i + 1 < argc) {
            args.seed = static_cast<unsigned int>(std::stoul(argv[++i]));
        } else if (arg == "--output" && i + 1 < argc) {
            args.output = argv[++i];
        }
    }

    return args;
}

}

int headless_main(int argc, char** argv) {
    const HeadlessArgs args = parseArgs(argc, argv);

    Random::seed(args.seed);

    World world = SceneBuilder::buildWorld();
    Population population = SceneBuilder::buildPopulation(world);
    GeneticAlgorithm ga;
    Simulation simulation(world, population);
    Trainer trainer(simulation, ga);

    simulation.startGeneration();
    Simulation::printGenerationStatsHeader();

    while (simulation.getGeneration() < args.generations) {
        trainer.step(1, population);
    }

    simulation.getStatistics().exportToCSV(args.output);

    std::cout << "\nHeadless run zavrsen: " << args.generations
              << " generacija (seed " << args.seed << "), izvezeno u "
              << args.output << "\n";

    return 0;
}
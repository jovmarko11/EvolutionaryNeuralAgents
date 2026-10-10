//
// Created by Marko Jovanovic on 22. 9. 2026..
//

#include "HeadlessRunner.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "model/simulation/SceneBuilder.h"
#include "app/config/Params.h"
#include "app/config/ExperimentConfig.h"
#include "core/math/Random.h"
#include "learning/statistics/ConsoleReporter.h"

#include "Trainer.h"

namespace {

struct HeadlessArgs {
    std::optional<std::string> configPath;
    std::optional<int> generationsOverride;
    std::optional<unsigned int> seedOverride;
};

HeadlessArgs parseArgs(int argc, char** argv) {
    HeadlessArgs args;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--config" && i + 1 < argc) {
            args.configPath = argv[++i];
        } else if (arg == "--generations" && i + 1 < argc) {
            args.generationsOverride = std::stoi(argv[++i]);
        } else if (arg == "--seed" && i + 1 < argc) {
            args.seedOverride = static_cast<unsigned int>(std::stoul(argv[++i]));
        }
    }

    return args;
}

// Jedinstveno ime foldera po eksperimentu: vreme pokretanja + seed,
// da se runovi nikad ne prepisu jedan preko drugog.
std::string makeExperimentId(unsigned int seed) {
    const auto now = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S") << "_seed" << seed;
    return oss.str();
}

}

int headless_main(int argc, char** argv) {
    const HeadlessArgs args = parseArgs(argc, argv);

    ExperimentConfig config;

    if (args.configPath) {
        std::ifstream configIn(*args.configPath);
        if (!configIn.is_open()) {
            std::cerr << "Ne mogu da otvorim config fajl: " << *args.configPath << "\n";
            return 1;
        }
        nlohmann::json configJson;
        configIn >> configJson;
        config = configJson.get<ExperimentConfig>();
    }

    // CLI flagovi imaju prednost nad configom - zgodno za brzu izmenu
    // jednog parametra bez pravljenja novog JSON fajla.
    if (args.generationsOverride) config.generations = *args.generationsOverride;
    if (args.seedOverride)        config.seed        = *args.seedOverride;

    const std::string experimentId = makeExperimentId(config.seed);
    const std::filesystem::path experimentDir = std::filesystem::path("experiments") / experimentId;
    std::filesystem::create_directories(experimentDir);

    // Config se upisuje POSLE CLI override-a, da fajl u folderu eksperimenta
    // uvek tacno opisuje sta se stvarno desilo tokom tog runa.
    {
        std::ofstream configOut(experimentDir / "config.json");
        configOut << nlohmann::json(config).dump(4);
    }

    Random::seed(config.seed);

    const FitnessWeights weights{
        config.fitnessProgress, config.fitnessSurvival,
        config.fitnessGoal, config.fitnessCollision, config.fitnessSpeed
    };

    World world = SceneBuilder::buildWorld();
    Population population = SceneBuilder::buildPopulation(world, config.numAgents);
    GeneticAlgorithm ga(config.elitismRatio, config.tournamentSize,
                         config.mutationRatio, config.mutationStrength);
    Simulation simulation(world, population, config.maxSteps, weights);
    Trainer trainer(simulation, ga, weights);

    simulation.startGeneration();
    ConsoleReporter::printHeader();

    while (simulation.getGeneration() < config.generations) {
        trainer.step(1, population);
    }

    const std::filesystem::path statsPath = experimentDir / "statistics.csv";
    simulation.getStatistics().exportToCSV(statsPath.string());

    std::cout << "\nHeadless run zavrsen: " << config.generations
              << " generacija (seed " << config.seed << ")\n"
              << "Rezultati u: " << experimentDir.string() << "\n";

    return 0;
}

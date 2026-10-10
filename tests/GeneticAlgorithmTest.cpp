#include <catch2/catch_test_macros.hpp>
#include "learning/evolution/GeneticAlgorithm.h"
#include "learning/evolution/Selection.h"
#include "learning/evolution/Crossover.h"
#include "learning/evolution/Mutation.h"
#include "core/math/Random.h"

#include <cmath>

TEST_CASE("Mutation postuje mutationRatio", "[mutation][evolution]") {

    SECTION("mutationRatio = 0 nikad ne menja genom") {
        Random::seed(42);
        std::vector<double> genome = {1.0, 2.0, 3.0, 4.0, 5.0};
        std::vector<double> original = genome;

        Mutation mutation(0.0, 1.0);
        mutation.mutate(genome);

        REQUIRE(genome == original);
    }

    SECTION("mutationRatio = 1 skoro sigurno menja svaki gen") {
        Random::seed(42);
        std::vector<double> genome(50, 0.0);
        std::vector<double> original = genome;

        Mutation mutation(1.0, 0.5);
        mutation.mutate(genome);

        int changed = 0;
        for (std::size_t i = 0; i < genome.size(); i++)
            if (genome[i] != original[i]) changed++;

        // mutationRatio=1 pokusava da mutira svaki gen; sansa da svih 50
        // Gaussian uzoraka ispadne bas 0.0 je prakticno nula
        REQUIRE(changed > 0);
    }
}

TEST_CASE("Crossover::uniform pravi validno dete", "[crossover][evolution]") {
    Random::seed(42);

    std::vector<double> a = {1, 1, 1, 1, 1};
    std::vector<double> b = {2, 2, 2, 2, 2};

    std::vector<double> child = Crossover::uniform(a, b);

    REQUIRE(child.size() == a.size());
    for (double gene : child)
        REQUIRE((gene == 1.0 || gene == 2.0));   // svaki gen dolazi ili od a ili od b
}

TEST_CASE("Selection::tournament vraca clana populacije", "[selection][evolution]") {
    Random::seed(42);

    std::vector<Individual> population = {
        {{0}, 10.0}, {{0}, 20.0}, {{0}, 5.0}, {{0}, 30.0}, {{0}, 15.0}
    };

    const Individual& picked = Selection(1).tournament(population);

    bool foundInPopulation = false;
    for (const auto& ind : population)
        if (&ind == &picked) foundInPopulation = true;

    REQUIRE(foundInPopulation);
}

TEST_CASE("Selection::tournament sa velikim uzorkom bira najbolju jedinku", "[selection][evolution]") {
    Random::seed(42);

    std::vector<Individual> population = {
        {{0}, 10.0}, {{0}, 20.0}, {{0}, 5.0}, {{0}, 30.0}, {{0}, 15.0}
    };

    Selection selection(static_cast<int>(population.size()));

    bool bestEverPicked = false;
    for (int i = 0; i < 200 && !bestEverPicked; i++) {
        const Individual& picked = selection.tournament(population);
        if (picked.fitness == 30.0) bestEverPicked = true;
    }

    REQUIRE(bestEverPicked);
}

TEST_CASE("GeneticAlgorithm::nextGeneration invarijante", "[genetic][evolution]") {
    Random::seed(42);

    std::vector<Individual> population;
    for (int i = 0; i < 20; i++)
        population.push_back(Individual{
            std::vector<double>{static_cast<double>(i), 0.0, 0.0},
            static_cast<double>(i)
        });
    // fitness raste sa i: 0,1,...,19 -> najbolja jedinka: fitness=19, genom {19,0,0}

    GeneticAlgorithm ga(/*elitismRatio*/ 0.1, /*tournamentSize*/ 3,
                         /*mutationRatio*/ 0.2, /*mutationStrength*/ 0.2);

    auto nextGen = ga.nextGeneration(population);

    SECTION("velicina populacije je ocuvana") {
        REQUIRE(nextGen.size() == population.size());
    }

    SECTION("elite se kopira netaknuto, sortirano opadajuce po fitnesu") {
        const std::size_t eliteCount = static_cast<std::size_t>(std::round(population.size() * 0.1));
        REQUIRE(eliteCount == 2);

        CHECK(nextGen[0] == std::vector<double>{19.0, 0.0, 0.0});
        CHECK(nextGen[1] == std::vector<double>{18.0, 0.0, 0.0});
    }

    SECTION("svi genomi imaju istu duzinu kao roditelji") {
        for (const auto& genome : nextGen)
            REQUIRE(genome.size() == 3);
    }
}

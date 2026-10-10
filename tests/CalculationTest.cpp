#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "learning/statistics/Calculation.h"

using Catch::Approx;

namespace {
    // steps, maxSteps=1000, distanceTravelled=0 (nekoriscen od strane Fitness/Calculation),
    // initialDistanceToTarget, bestDistanceToTarget, crashed, reachedTarget, timeOut
    EpisodeResult makeResult(int steps, double initDist, double bestDist,
                              bool crashed, bool reached, bool timeout) {
        return EpisodeResult{steps, 1000, 0.0, initDist, bestDist, crashed, reached, timeout};
    }
}

TEST_CASE("Calculation::calculate outcome counts", "[calculation][statistics]") {
    std::vector<EpisodeResult> results = {
        makeResult(500, 100.0, 50.0, true,  false, false),   // crash
        makeResult(500, 100.0, 50.0, true,  false, false),   // crash
        makeResult(300, 100.0, 0.0,  false, true,  false),   // reach
        makeResult(1000, 100.0, 60.0, false, false, true),   // timeout
    };

    Calculation calc;
    const FitnessWeights weights{};
    GenerationStatistics gs = calc.calculate(7, results, weights);

    REQUIRE(gs.generation == 7);
    CHECK(gs.reach == 1);
    CHECK(gs.crash == 2);
    CHECK(gs.tout == 1);
    CHECK(gs.reach + gs.crash + gs.tout == static_cast<int>(results.size()));
}

TEST_CASE("Calculation::calculate fitness distribucija (paran broj uzoraka)", "[calculation][statistics]") {
    // svi timeout, bez crasha/gola -> fitness = 10 * progress(), lako za rucnu proveru
    std::vector<EpisodeResult> results = {
        makeResult(0, 1.0, 0.0, false, false, true),   // progress=1  -> fitness=10
        makeResult(0, 2.0, 0.0, false, false, true),   // progress=2  -> fitness=20
        makeResult(0, 3.0, 0.0, false, false, true),   // progress=3  -> fitness=30
        makeResult(0, 4.0, 0.0, false, false, true),   // progress=4  -> fitness=40
    };

    Calculation calc;
    const FitnessWeights weights{};
    GenerationStatistics gs = calc.calculate(0, results, weights);

    CHECK(gs.worstFitness == Approx(10.0));
    CHECK(gs.bestFitness == Approx(40.0));
    CHECK(gs.meanFitness == Approx(25.0));
    CHECK(gs.medianFitness == Approx(25.0));   // (20+30)/2, paran broj uzoraka
}

TEST_CASE("Calculation::calculate fitness distribucija (neparan broj uzoraka)", "[calculation][statistics]") {
    std::vector<EpisodeResult> results = {
        makeResult(0, 1.0, 0.0, false, false, true),   // fitness=10
        makeResult(0, 2.0, 0.0, false, false, true),   // fitness=20
        makeResult(0, 3.0, 0.0, false, false, true),   // fitness=30
        makeResult(0, 4.0, 0.0, false, false, true),   // fitness=40
        makeResult(0, 5.0, 0.0, false, false, true),   // fitness=50
    };

    Calculation calc;
    const FitnessWeights weights{};
    GenerationStatistics gs = calc.calculate(0, results, weights);

    CHECK(gs.medianFitness == Approx(30.0));   // sredisnji element, neparan broj uzoraka
}

TEST_CASE("Calculation::calculate bestBreakdown odgovara stvarno najboljem rezultatu", "[calculation][statistics]") {
    std::vector<EpisodeResult> results = {
        makeResult(500, 100.0, 80.0, true,  false, false),   // crash, progress=20 -> fitness negativan/nizak
        makeResult(200, 100.0, 0.0,  false, true,  false),   // reach, progress=100 -> ubedljivo najbolji
        makeResult(1000, 100.0, 60.0, false, false, true),   // timeout, progress=40
    };

    Calculation calc;
    const FitnessWeights weights{};
    GenerationStatistics gs = calc.calculate(0, results, weights);

    // rucno racunamo breakdown za rezultat za koji ZNAMO da je najbolji (indeks 1)
    FitnessBreakdown expected = fitnessBreakdown(results[1], weights);

    CHECK(gs.bestFitness == Approx(expected.total()));
    CHECK(gs.bestBreakdown.total() == Approx(expected.total()));
    CHECK(gs.bestBreakdown.goal == Approx(expected.goal));
    CHECK(gs.bestBreakdown.speed == Approx(expected.speed));
}

TEST_CASE("Calculation::calculate step statistike", "[calculation][statistics]") {
    std::vector<EpisodeResult> results = {
        makeResult(100, 10.0, 5.0, false, false, true),
        makeResult(400, 10.0, 5.0, false, false, true),
        makeResult(700, 10.0, 5.0, false, false, true),
    };

    Calculation calc;
    const FitnessWeights weights{};
    GenerationStatistics gs = calc.calculate(0, results, weights);

    CHECK(gs.minStepsTaken == 100);
    CHECK(gs.maxStepsTaken == 700);
    CHECK(gs.averageSteps == Approx((100.0 + 400.0 + 700.0) / 3.0));
}

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../fitness/Fitness.h"

using Catch::Approx;

TEST_CASE("Fitness breakdown po granama formule", "[fitness]") {
    const FitnessWeights weights{};

    SECTION("crash bez cilja: negativan collision, nema goal/speed") {
        EpisodeResult r{50, 1000, 20.0, 100.0, 60.0, true, false, false};
        FitnessBreakdown b = fitnessBreakdown(r, weights);

        CHECK(b.progress == Approx(weights.progress * 40.0));  // 100 - 60
        CHECK(b.collision == Approx(-weights.collision));
        CHECK(b.goal == 0.0);
        CHECK(b.speed == 0.0);
    }

    SECTION("reachedTarget: goal + speed bonus proporcionalan preostalim koracima") {
        EpisodeResult r{200, 1000, 90.0, 100.0, 0.0, false, true, false};
        FitnessBreakdown b = fitnessBreakdown(r, weights);

        CHECK(b.goal == Approx(weights.goal));
        CHECK(b.collision == 0.0);
        const double expectedRemaining = (1000.0 - 200.0) / 1000.0;
        CHECK(b.speed == Approx(weights.speed * expectedRemaining));
    }

    SECTION("timeout bez crash-a: nema penal, nema bonus") {
        EpisodeResult r{1000, 1000, 30.0, 100.0, 70.0, false, false, true};
        FitnessBreakdown b = fitnessBreakdown(r, weights);

        CHECK(b.collision == 0.0);
        CHECK(b.goal == 0.0);
        CHECK(b.speed == 0.0);
    }

    SECTION("total() je zbir svih komponenti") {
        EpisodeResult r{200, 1000, 90.0, 100.0, 0.0, false, true, false};
        FitnessBreakdown b = fitnessBreakdown(r, weights);

        CHECK(b.total() == Approx(b.progress + b.survival + b.goal + b.collision + b.speed));
        CHECK(fitness(r, weights) == Approx(b.total()));
    }
}

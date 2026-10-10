//
// Created by Marko Jovanovic on 30. 9. 2026..
//
#include "../configuration/ExperimentConfig.h"

#include "catch2/catch_test_macros.hpp"
TEST_CASE("ExperimentConfig JSON round-trip", "[config][io]") {
    ExperimentConfig original;
    original.mutationRatio = 0.35;
    original.numAgents = 250;
    original.mutationStrength = 0.87;
    original.generations = 300;

    nlohmann::json j = original;
    ExperimentConfig restored = j.get<ExperimentConfig>();

    REQUIRE(restored == original);
}
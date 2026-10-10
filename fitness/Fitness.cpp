//
// Created by Marko Jovanovic on 18. 9. 2026..
//
#include "Fitness.h"

FitnessBreakdown fitnessBreakdown(const EpisodeResult& result, const FitnessWeights& weights) {
    FitnessBreakdown b;

    b.progress  = weights.progress * result.progress();
    b.survival  = weights.survival * result.steps;
    b.goal      = result.reachedTarget ? weights.goal : 0.0;
    b.collision = result.crashed ? -weights.collision : 0.0;

    if (result.reachedTarget) {
        const double remainingFraction = static_cast<double>(result.maxSteps - result.steps) / result.maxSteps;
        b.speed = weights.speed * remainingFraction;
    }

    return b;
}

//
// Created by Marko Jovanovic on 18. 9. 2026..
//
#include "Fitness.h"

FitnessBreakdown fitnessBreakdown(const EpisodeResult& result) {
    FitnessBreakdown b;

    b.progress  = FitnessWeights::progress * result.progress();
    b.survival  = FitnessWeights::survival * result.steps;
    b.goal      = result.reachedTarget ? FitnessWeights::goal : 0.0;
    b.collision = result.crashed ? -FitnessWeights::collision : 0.0;

    return b;
}

//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#include "Calculation.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

GenerationStatistics Calculation::calculate(int generation, const std::vector<EpisodeResult>& results) {
    assert(!results.empty());

    GenerationStatistics gs{};
    gs.generation = generation;

    const std::vector<double> fitnessValues = computeFitnessValues(results);

    computeFitnessDistribution(fitnessValues, gs);
    computeOutcomeCounts(results, gs);
    computeProgressStats(results, gs);
    computeStepStats(results, gs);
    gs.bestBreakdown = findBestBreakdown(results, fitnessValues);

    return gs;
}

std::vector<double> Calculation::computeFitnessValues(const std::vector<EpisodeResult>& results) {
    std::vector<double> values;
    values.reserve(results.size());

    for (const EpisodeResult& r : results) {
        values.push_back(fitness(r));
    }

    return values;
}

void Calculation::computeFitnessDistribution(const std::vector<double>& fitnessValues, GenerationStatistics& gs) {
    std::vector<double> sorted = fitnessValues;
    std::sort(sorted.begin(), sorted.end());

    const std::size_t n = sorted.size();
    const double mean = std::accumulate(sorted.begin(), sorted.end(), 0.0) / static_cast<double>(n);

    double variance = 0.0;
    for (const double v : sorted) {
        variance += (v - mean) * (v - mean);
    }
    variance /= static_cast<double>(n);

    gs.worstFitness = sorted.front();
    gs.bestFitness = sorted.back();
    gs.meanFitness = mean;
    gs.medianFitness = (n % 2)
        ? sorted[n / 2]
        : 0.5 * (sorted[n / 2 - 1] + sorted[n / 2]);
    gs.stdevFitness = std::sqrt(variance);
}

void Calculation::computeOutcomeCounts(const std::vector<EpisodeResult>& results, GenerationStatistics& gs) {
    gs.reach = 0;
    gs.crash = 0;
    gs.tout = 0;

    for (const EpisodeResult& r : results) {
        gs.reach += r.reachedTarget;
        gs.crash += r.crashed;
        gs.tout  += r.timeOut;
    }
}

void Calculation::computeProgressStats(const std::vector<EpisodeResult>& results, GenerationStatistics& gs) {
    double sumProgress = 0.0;
    double maxProgress = 0.0;

    for (const EpisodeResult& r : results) {
        const double p = r.progress();
        sumProgress += p;
        maxProgress = std::max(maxProgress, p);
    }

    gs.maxProgress = maxProgress;
    gs.meanProgress = sumProgress / static_cast<double>(results.size());
}

void Calculation::computeStepStats(const std::vector<EpisodeResult>& results, GenerationStatistics& gs) {
    long long sumSteps = 0;
    int minSteps = results.front().steps;
    int maxStepsTaken = results.front().steps;

    for (const EpisodeResult& r : results) {
        sumSteps += r.steps;
        minSteps = std::min(minSteps, r.steps);
        maxStepsTaken = std::max(maxStepsTaken, r.steps);
    }

    gs.averageSteps = static_cast<double>(sumSteps) / static_cast<double>(results.size());
    gs.minStepsTaken = minSteps;
    gs.maxStepsTaken = maxStepsTaken;
}

FitnessBreakdown Calculation::findBestBreakdown(const std::vector<EpisodeResult>& results,
    const std::vector<double>& fitnessValues) {
    const auto bestIt = std::max_element(fitnessValues.begin(), fitnessValues.end());
    const std::size_t bestIndex = static_cast<std::size_t>(std::distance(fitnessValues.begin(), bestIt));

    return fitnessBreakdown(results[bestIndex]);
}

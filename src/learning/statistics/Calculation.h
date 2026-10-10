//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_CALCULATION_H
#define EVOLUTIONARYNEURALAGENTS_CALCULATION_H
#include <vector>

#include "GenerationStatistics.h"
#include "model/simulation/Episode.h"
#include "learning/fitness/Fitness.h"

class Calculation {
public:
    GenerationStatistics calculate(int generation, const std::vector<EpisodeResult>& results, const FitnessWeights& weights);

private:
    static std::vector<double> computeFitnessValues(const std::vector<EpisodeResult>& results, const FitnessWeights& weights);

    static void computeFitnessDistribution(const std::vector<double>& fitnessValues, GenerationStatistics& gs);
    static void computeOutcomeCounts(const std::vector<EpisodeResult>& results, GenerationStatistics& gs);
    static void computeProgressStats(const std::vector<EpisodeResult>& results, GenerationStatistics& gs);
    static void computeStepStats(const std::vector<EpisodeResult>& results, GenerationStatistics& gs);
    static FitnessBreakdown findBestBreakdown(const std::vector<EpisodeResult>& results, const std::vector<double>& fitnessValues, const FitnessWeights& weights);
};


#endif //EVOLUTIONARYNEURALAGENTS_CALCULATION_H

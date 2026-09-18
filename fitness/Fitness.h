//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_FITNESS_H
#define EVOLUTIONARYNEURALAGENTS_FITNESS_H
#include "../simulation/Episode.h"

namespace FitnessWeights {
    inline constexpr double progress = 10.0;
    inline constexpr double survival = 0.05;
    inline constexpr double goal = 1000.0;
    inline constexpr double collision = 50.0;
}

struct FitnessBreakdown {
    double progress = 0;
    double survival = 0;
    double goal = 0;
    double collision = 0;   // negativan

    double total() const { return progress + survival + goal + collision; }
};

FitnessBreakdown fitnessBreakdown(const EpisodeResult& result);

inline double fitness(const EpisodeResult& result) {
    return fitnessBreakdown(result).total();
}


#endif //EVOLUTIONARYNEURALAGENTS_FITNESS_H
//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_FITNESS_H
#define EVOLUTIONARYNEURALAGENTS_FITNESS_H
#include "../simulation/Episode.h"

struct FitnessWeights {
    double progress = 10.0;
    double survival = 0.02;
    double goal = 1000.0;
    double collision = 50.0;
    double speed = 400.0;

    bool operator==(const FitnessWeights&) const = default;
};

struct FitnessBreakdown {
    double progress = 0;
    double survival = 0;
    double goal = 0;
    double collision = 0;   // negativan
    double speed = 0;

    double total() const { return progress + survival + goal + collision + speed; }
};

FitnessBreakdown fitnessBreakdown(const EpisodeResult& result, const FitnessWeights& weights);

inline double fitness(const EpisodeResult& result, const FitnessWeights& weights) {
    return fitnessBreakdown(result, weights).total();
}


#endif //EVOLUTIONARYNEURALAGENTS_FITNESS_H

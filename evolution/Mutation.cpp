//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "Mutation.h"
#include "../math/Random.h"

Mutation::Mutation(double mutationRatio, double mutationStrength)
    :   mutationRatio(mutationRatio),
        mutationStrength(mutationStrength)  {}


void Mutation::mutate(std::vector<double>& genome) const {
    for (double & gene : genome) {
        if (!Random::bernoulli(mutationRatio)) continue;

        gene += Random::normal(0, mutationStrength);
    }
}
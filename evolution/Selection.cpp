//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "Selection.h"
#include "../math/Random.h"
#include <cassert>

Selection::Selection(int tournamentSize) : tournamentSize(tournamentSize)   {}


const Individual& Selection::tournament(const std::vector<Individual> &population) const{
    assert(!population.empty());
    assert(tournamentSize >= 1 && tournamentSize <= static_cast<int>(population.size()));

    const Individual* best = nullptr;

    for (int i = 0; i < tournamentSize; i++) {
        int idx = Random::uniformInt(0, static_cast<int>(population.size()) - 1);
        const Individual& candidate = population[idx];

        if (!best || candidate.fitness > best->fitness) {
            best = &candidate;
        }
    }
    return *best;
}



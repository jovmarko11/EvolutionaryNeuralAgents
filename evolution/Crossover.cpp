//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#include "Crossover.h"
#include "../math/Random.h"

#include <cassert>

std::vector<double> Crossover::uniform(const std::vector<double> &a, const std::vector<double> &b) {
    assert(a.size() == b.size());
    std::vector<double> result;
    result.reserve(a.size());

    for (std::size_t i = 0; i < a.size(); i++) {
        if (Random::bernoulli(0.5)) {
            result.push_back(a[i]);
        }
        else {
            result.push_back(b[i]);
        }
    }
    assert(result.size() == a.size());
    return result;
}

std::vector<double> Crossover::blend(const std::vector<double> &a, const std::vector<double> &b) {
    assert(a.size() == b.size());
    std::vector<double> result;
    result.reserve(a.size());

    for (std::size_t i = 0; i < a.size(); i++) {
        double alpha = Random::uniform(0.0, 1.0);
        result.push_back(alpha * a[i] + (1 - alpha) * b[i]);
    }
    assert(result.size() == a.size());
    return result;
}


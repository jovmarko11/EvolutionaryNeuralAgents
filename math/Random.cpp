//
// Created by Marko Jovanovic on 17. 9. 2026..
//

#include "Random.h"

#include "../configuration/Params.h"

namespace {
    std::mt19937 generator{Params::randomSeed};
}

namespace Random {

    void seed(unsigned int value) {
        generator.seed(value);
    }

    std::mt19937& engine() {
        return generator;
    }

    double uniform(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(generator);
    }

    double normal(double mean, double stddev) {
        std::normal_distribution<double> dist(mean, stddev);
        return dist(generator);
    }

    int uniformInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(generator);
    }

    bool bernoulli(double p) {
        double u = uniform(0.0, 1.0);
        if (u <= p) return true;
        return false;
    }

}

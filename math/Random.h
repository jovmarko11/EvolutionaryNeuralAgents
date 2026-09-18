//
// Created by Marko Jovanovic on 17. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_RANDOM_H
#define EVOLUTIONARYNEURALAGENTS_RANDOM_H

#include <random>

namespace Random {

    void seed(unsigned int value);

    std::mt19937& engine();

    double uniform(double min, double max);
    double normal(double mean, double stddev);
    int uniformInt(int min, int max);

}

#endif //EVOLUTIONARYNEURALAGENTS_RANDOM_H

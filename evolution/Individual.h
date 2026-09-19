//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_INDIVIDUAL_H
#define EVOLUTIONARYNEURALAGENTS_INDIVIDUAL_H
#include <vector>

struct Individual {
    std::vector<double> genome;
    double fitness = 0.0;
};

#endif //EVOLUTIONARYNEURALAGENTS_INDIVIDUAL_H
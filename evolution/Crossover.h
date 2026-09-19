//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_CROSSOVER_H
#define EVOLUTIONARYNEURALAGENTS_CROSSOVER_H
#include <vector>


class Crossover {
public:
    static std::vector<double> uniform(const std::vector<double>& a, const std::vector<double>& b);

    static std::vector<double> blend(const std::vector<double>& a, const std::vector<double>& b);
};


#endif //EVOLUTIONARYNEURALAGENTS_CROSSOVER_H
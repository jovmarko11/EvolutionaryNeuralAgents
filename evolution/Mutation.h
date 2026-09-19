//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_MUTATION_H
#define EVOLUTIONARYNEURALAGENTS_MUTATION_H
#include <vector>


class Mutation {
    double mutationRatio, mutationStrength;

public:
    Mutation(double mutationRatio, double mutationStrength);

    void mutate(std::vector<double>& genome) const;
};


#endif //EVOLUTIONARYNEURALAGENTS_MUTATION_H
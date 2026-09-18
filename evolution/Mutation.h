//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_MUTATION_H
#define EVOLUTIONARYNEURALAGENTS_MUTATION_H
#include "../simulation/Population.h"


class Mutation {

    public:
    Mutation (Population& population);

    void mutate();

};


#endif //EVOLUTIONARYNEURALAGENTS_MUTATION_H
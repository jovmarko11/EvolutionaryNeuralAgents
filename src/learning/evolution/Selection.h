//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_SELECTION_H
#define EVOLUTIONARYNEURALAGENTS_SELECTION_H
#include <vector>

#include "Individual.h"


class Selection {
    int tournamentSize;
public:
    explicit Selection(int tournamentSize);

    const Individual& tournament(const std::vector<Individual>& population) const;


    void setTournamentSize(const int ts) { this->tournamentSize = ts; }
    int getTournamentSize() const { return this->tournamentSize; }
};


#endif //EVOLUTIONARYNEURALAGENTS_SELECTION_H
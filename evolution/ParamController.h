//
// Created by Marko Jovanovic on 8. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PARAMCONTROLLER_H
#define EVOLUTIONARYNEURALAGENTS_PARAMCONTROLLER_H
#include "GeneticAlgorithm.h"
#include "../statistics/Statistics.h"

class ParamController {
    GeneticAlgorithm& ga;
    Statistics& stats;

    double updateElitismRatio(const GenerationStatistics&  gs);
    double updateMutationRatio(const GenerationStatistics& gs);
    double updateMutationStrength(const GenerationStatistics& gs);
    int updateTournamentSize(const GenerationStatistics& gs);

public:
    ParamController(GeneticAlgorithm& ga, Statistics& stats) : ga(ga), stats(stats) {}

    void updateParameters(const GenerationStatistics& gs);
};


#endif //EVOLUTIONARYNEURALAGENTS_PARAMCONTROLLER_H
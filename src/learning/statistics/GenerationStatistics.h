//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_GENERATIONSTATISTICS_H
#define EVOLUTIONARYNEURALAGENTS_GENERATIONSTATISTICS_H
#include "learning/fitness/Fitness.h"
#include "learning/evolution/GAParams.h"

struct GenerationStatistics {
    int generation = 0;

    // Raspodela fitnesa cele generacije
    double bestFitness = 0.0;
    double meanFitness = 0.0;
    double medianFitness = 0.0;
    double worstFitness = 0.0;
    double stdevFitness = 0.0;

    int reach = 0;
    int crash = 0;
    int tout = 0;

    double maxProgress = 0.0;
    double meanProgress = 0.0;


    double averageSteps = 0.0;
    int minStepsTaken = 0;
    int maxStepsTaken = 0;

    FitnessBreakdown bestBreakdown{};

    // Parametri GA kojima je OVA generacija proizvedena (snapshot uzet pre
    // eventualne adaptacije za sledecu). Za generaciju 0, koja je nasumicna,
    // to su pocetne vrednosti.
    GAParams gaParams{};
};


#endif //EVOLUTIONARYNEURALAGENTS_GENERATIONSTATISTICS_H

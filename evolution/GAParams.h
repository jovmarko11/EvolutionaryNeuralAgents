//
// Created by Marko Jovanovic on 8. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_GAPARAMS_H
#define EVOLUTIONARYNEURALAGENTS_GAPARAMS_H

#include "../configuration/Params.h"

// Parametri genetskog algoritma kao obican podatak, bez zavisnosti od
// GeneticAlgorithm klase - da ih mogu koristiti i statistika i ispis.
struct GAParams {
    double elitismRatio = Params::elitismRatio;
    double mutationRatio = Params::mutationRatio;
    double mutationStrength = Params::mutationStrength;
    int tournamentSize = Params::tournamentSize;

    bool operator==(const GAParams&) const = default;
};

#endif //EVOLUTIONARYNEURALAGENTS_GAPARAMS_H

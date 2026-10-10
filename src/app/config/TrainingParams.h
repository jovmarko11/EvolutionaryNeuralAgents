#ifndef EVOLUTIONARYNEURALAGENTS_TRAININGPARAMS_H
#define EVOLUTIONARYNEURALAGENTS_TRAININGPARAMS_H

#include "learning/evolution/GAParams.h"
#include "learning/fitness/Fitness.h"

// Parametri treninga koji se mogu menjati u toku rada (na granici generacije).
// Obican podatak: nema zavisnosti od Trainer/GA klasa.
struct TrainingParams {
    GAParams ga;
    FitnessWeights fitness;

    bool operator==(const TrainingParams&) const = default;
};

#endif //EVOLUTIONARYNEURALAGENTS_TRAININGPARAMS_H

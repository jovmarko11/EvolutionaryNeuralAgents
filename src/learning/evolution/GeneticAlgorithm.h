//
// Created by Marko Jovanovic on 18. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
#define EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
#include "Crossover.h"
#include "GAParams.h"
#include "Individual.h"
#include "Mutation.h"
#include "Selection.h"
#include "model/simulation/Population.h"
#include "app/config/Params.h"


class GeneticAlgorithm {
    double elitismRatio;

    Mutation mutation;
    Selection selection;

public:
    explicit GeneticAlgorithm(double elitismRatio = Params::elitismRatio,
                    int tournamentSize = Params::tournamentSize,
                    double mutationRatio = Params::mutationRatio,
                    double mutationStrength = Params::mutationStrength);

    std::vector<std::vector<double>> nextGeneration(const std::vector<Individual>& current) const;

    void setElitismRatio(double elitismRatio);
    void setMutationRatio(double mutationRatio);
    void setMutationStrength(double mutationStrength);
    void setTournamentSize(int tournamentSize);

    double getElitismRatio() const;
    double getMutationRatio() const;
    double getMutationStrength() const;
    int getTournamentSize() const;

    GAParams getParams() const;
    void setParams(const GAParams& params);
};

inline void GeneticAlgorithm::setElitismRatio(double er) { this->elitismRatio = er; }
inline void GeneticAlgorithm::setMutationRatio(double mr) { mutation.setMutationRatio(mr); }
inline void GeneticAlgorithm::setMutationStrength(double ms) { mutation.setMutationStrength(ms); }
inline void GeneticAlgorithm::setTournamentSize(int ts) { selection.setTournamentSize(ts); }

inline double GeneticAlgorithm::getElitismRatio() const { return this->elitismRatio; }
inline double GeneticAlgorithm::getMutationRatio() const { return mutation.getMutationRatio(); }
inline double GeneticAlgorithm::getMutationStrength() const { return mutation.getMutationStrength(); }
inline int GeneticAlgorithm::getTournamentSize() const { return selection.getTournamentSize(); }

inline void GeneticAlgorithm::setParams(const GAParams& p) {
    setElitismRatio(p.elitismRatio);
    setMutationRatio(p.mutationRatio);
    setMutationStrength(p.mutationStrength);
    setTournamentSize(p.tournamentSize);
}

inline GAParams GeneticAlgorithm::getParams() const {
    return GAParams{ getElitismRatio(), getMutationRatio(), getMutationStrength(), getTournamentSize() };
}








#endif //EVOLUTIONARYNEURALAGENTS_GENETICALGORITHM_H
//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_POPULATION_H
#define EVOLUTIONARYNEURALAGENTS_POPULATION_H
#include <vector>
#include "../world/Agent.h"

class Population {
    std::vector<Agent> agents;

public:
    std::vector<Agent>& getAgents();
    const std::vector<Agent>& getAgents() const;
    std::size_t size() const;

    void addAgent(Agent agent);

    std::vector<double> getFitnesses();

};

inline std::vector<Agent>& Population::getAgents() { return agents; }
inline const std::vector<Agent>& Population::getAgents() const { return agents; }

#endif //EVOLUTIONARYNEURALAGENTS_POPULATION_H

//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#include "Population.h"

void Population::addAgent(Agent agent) {
    agents.push_back(std::move(agent));
}

std::size_t Population::size() const {
    return agents.size();
}

std::vector<double> Population::getFitnesses() {
    std::vector<double> fitnesses;

    for (auto agent : agents) {

    }
}
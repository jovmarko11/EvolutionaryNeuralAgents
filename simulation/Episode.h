//
// Created by Marko Jovanovic on 16. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_EPISODE_H
#define EVOLUTIONARYNEURALAGENTS_EPISODE_H
#include "../world/Agent.h"

struct EpisodeResult {
    int steps;
    int maxSteps;
    double distanceTravelled;
    double initialDistanceToTarget;
    double bestDistanceToTarget;
    bool crashed;
    bool reachedTarget;
    bool timeOut;

    double progress() const {
        return initialDistanceToTarget - bestDistanceToTarget;
    }
};

class Episode {
    const World& world;
    Agent& agent;
    int maxSteps;
    EpisodeResult result;
    bool finished;

public:
    Episode(const World& world, Agent& agent, int maxSteps);

    Episode(const Episode&) = delete;
    Episode& operator=(const Episode&) = delete;
    Episode(Episode&&) = default;

    bool step();
    const EpisodeResult& run();
    const EpisodeResult& getResult() const;
    bool isFinished() const;
};


#endif //EVOLUTIONARYNEURALAGENTS_EPISODE_H
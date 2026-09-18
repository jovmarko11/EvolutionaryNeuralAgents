//
// Created by Marko Jovanovic on 16. 9. 2026..
//

#include "Episode.h"

#include <algorithm>

#include "../world/World.h"


Episode::Episode(const World& world, Agent& agent, int maxSteps)
    :   world(world),
        agent(agent),
        maxSteps(maxSteps),
        result{},
        finished(false)
{
    // Agent se resetuje ovde, a ne spolja, tako da je nemoguce pokrenuti
    // epizodu nad agentom koji je ostao prljav od prethodne.
    agent.reset();

    const double distanceToTarget =
        (world.getTarget().getPosition() - agent.getPosition()).length();

    result.steps = 0;
    result.distanceTravelled = 0.0;
    result.initialDistanceToTarget = distanceToTarget;
    result.bestDistanceToTarget = distanceToTarget;
    result.crashed = false;
    result.reachedTarget = false;
    result.timeOut = false;

    // Degenerisana epizoda: nula koraka je odmah timeout.
    if (maxSteps <= 0) {
        result.timeOut = true;
        finished = true;
    }
}


bool Episode::step() {
    if (finished) {
        return false;
    }

    const Vec2 positionBefore = agent.getPosition();

    const SimStatus status = agent.step(world);

    const Vec2 positionAfter = agent.getPosition();

    // Statistika se akumulira ovde, ne u agentu.
    result.steps++;
    result.distanceTravelled += (positionAfter - positionBefore).length();

    const double distanceToTarget =
        (world.getTarget().getPosition() - positionAfter).length();

    // Najbolja, a ne poslednja distanca — inace se agentu isplati
    // da se priblizi cilju pa pobegne.
    result.bestDistanceToTarget =
        std::min(result.bestDistanceToTarget, distanceToTarget);

    // Terminalni ishodi. Redosled je bitan: sudar ima prioritet nad
    // istekom vremena, jer korak koji je ubio agenta jeste odigran.
    switch (status) {
        case SimStatus::Blocked:
        case SimStatus::OutOfBounds:
            result.crashed = true;
            finished = true;
            return false;

        case SimStatus::ReachedTarget:
            result.reachedTarget = true;
            finished = true;
            return false;

        case SimStatus::Ongoing:
            break;
    }

    if (result.steps >= maxSteps) {
        result.timeOut = true;
        finished = true;
        return false;
    }

    return true;
}


const EpisodeResult& Episode::run() {
    while (step()) {
        // headless: vrti do terminalnog stanja
    }

    return result;
}


bool Episode::isFinished() const {
    return finished;
}


const EpisodeResult& Episode::getResult() const {
    return result;
}

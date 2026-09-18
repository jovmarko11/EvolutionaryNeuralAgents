//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_SCENEBUILDER_H
#define EVOLUTIONARYNEURALAGENTS_SCENEBUILDER_H
#include "../world/World.h"
#include "Population.h"

class SceneBuilder {
public:
    static World buildWorld();

    // Trazi svet da bi proverio da je startna pozicija slobodna.
    static Population buildPopulation(const World& world);
};

#endif //EVOLUTIONARYNEURALAGENTS_SCENEBUILDER_H

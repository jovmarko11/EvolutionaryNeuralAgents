//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#include "SceneBuilder.h"

#include <stdexcept>
#include <numbers>
#include "../world/obstacles/CircleObstacle.h"
#include "../world/obstacles/RectObstacle.h"
#include "../configuration/Params.h"

World SceneBuilder::buildWorld() {
    World world(Params::worldLimitX, Params::worldLimitY);


    // --- zidovi: prolaz desno, pa levo, pa desno ---
    world.addObstacle(std::make_unique<RectObstacle>(Vec2(-8, -16), 24.0, 2.0));
    world.addObstacle(std::make_unique<RectObstacle>(Vec2( 8,  -4), 24.0, 2.0));
    world.addObstacle(std::make_unique<RectObstacle>(Vec2(-8,   8), 24.0, 2.0));

    // --- startni prostor ---
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2( -4, -24), 2.5));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2(  9, -26), 2.0));

    // --- prvi hodnik: slalom dole-gore-dole ---
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2(-13, -13), 1.8));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2( -2,  -7), 1.8));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2(  9, -13), 1.8));

    // --- drugi hodnik ---
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2(-15,  -1), 1.8));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2( -4,   5), 1.8));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2(  8,  -1), 1.8));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2( 17,   5), 1.5));

    // --- zavrsni prostor ---
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2( -9,  14), 2.5));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2(  2,  21), 3.0));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2( 16,  22), 2.5));
    world.addObstacle(std::make_unique<CircleObstacle>(Vec2(-16,  24), 2.0));

    world.setTarget(Target(Vec2(10, 15), 0.8));

    return world;
}


Population SceneBuilder::buildPopulation(const World& world) {
    const Vec2 spawn(Params::spawnX, Params::spawnY);
    const Vec2 heading = Vec2(1.0, 0.0).rotated(Params::spawnHeading);

    // Bolje pasti odmah nego pustiti agente koji umiru u prvom koraku.
    const double required = static_cast<double>(Params::agentRadius) + Params::spawnClearance;
    const double available = world.clearance(spawn);

    if (available < required) {
        throw std::runtime_error(
            "SceneBuilder: startna pozicija je preblizu prepreci ili granici sveta"
        );
    }

    Population population;
    for (int i = 0; i < Params::numAgents; i++) {
        population.addAgent(Agent(spawn, heading));
    }

    return population;
}

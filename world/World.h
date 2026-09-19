//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_WORLD_H
#define EVOLUTIONARYNEURALAGENTS_WORLD_H
#include <vector>
#include <memory>

#include "Obstacle.h"
#include "SimStatus.h"
#include "Target.h"
#include "../rendering/Drawable.h"
#include "../rendering/DrawParams.h"

class World : public Drawable {
private:
    double limitX, limitY;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    Target target;

    DrawParams borderParams;
    DrawParams gridParams;

    void drawBorder(sf::RenderWindow& window) const;
    void drawGrid(sf::RenderWindow& window) const;

    // Rastojanje do izlaska iz granica arene duz zraka.
    // Pretpostavlja da je origin unutar [-limitX,limitX] x [-limitY,limitY].
    double boundaryDistance(const Vec2& origin, const Vec2& direction) const;

public:
    World(double limitX, double limitY,
          DrawParams borderParams = DrawParams{{255, 255, 255, 255}, 0.1f},
          DrawParams gridParams   = DrawParams{{255, 255, 255, 255}, 0.f, 0.25f, 1.f});

    void addObstacle(std::unique_ptr<Obstacle> obstacle);
    void setTarget(Target target);

    // Tackasti upit. Isto sto i inRange(coordinates, 0.0).
    SimStatus inRange(const Vec2& coordinates) const;

    // Upit za telo agenta: disk poluprecnika radius oko centre.
    // Egzaktan, za razliku od ranijeg uzorkovanja sa pet tacaka.
    SimStatus inRange(const Vec2& centre, double radius) const;

    // Rastojanje do najblize prepreke ili granice sveta.
    // Negativno znaci da je tacka vec unutar necega.
    double clearance(const Vec2& point) const;

    double castRay(const Vec2 &origin, const Vec2 &direction, double maxDistance) const;


    double getLimitX() const;
    double getLimitY() const;
    const std::vector<std::unique_ptr<Obstacle>>& getObstacles() const;
    const Target& getTarget() const;

    void draw(sf::RenderWindow& window) const override;
};

inline double World::getLimitX() const { return limitX; }
inline double World::getLimitY() const { return limitY; }
inline const std::vector<std::unique_ptr<Obstacle>>& World::getObstacles() const { return obstacles; }
inline const Target& World::getTarget() const { return target; }

#endif //EVOLUTIONARYNEURALAGENTS_WORLD_H

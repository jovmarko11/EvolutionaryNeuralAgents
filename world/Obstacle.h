//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_OBSTACLE_H
#define EVOLUTIONARYNEURALAGENTS_OBSTACLE_H
#include "../math/Vec2.h"
#include "../rendering/Drawable.h"
#include "../rendering/DrawParams.h"

class Obstacle : public Drawable {
protected:
    Vec2 position;
    DrawParams drawParams;

public:
    Obstacle(Vec2 position, DrawParams drawParams);
    ~Obstacle() override = default;

    Vec2 getPosition() const;

    // Signed distance: < 0 unutra, 0 na ivici, > 0 napolju.
    // Jedina geometrija koju izvedene klase moraju da implementiraju.
    virtual double distanceTo(const Vec2& point) const = 0;

    // Tacka je u prepreci.
    bool contains(const Vec2& point) const;

    // Disk (centar, poluprecnik) dodiruje prepreku.
    bool overlaps(const Vec2& centre, double radius) const;
};

inline Vec2 Obstacle::getPosition() const { return position; }

inline bool Obstacle::contains(const Vec2& point) const {
    return distanceTo(point) <= 0.0;
}

inline bool Obstacle::overlaps(const Vec2& centre, double radius) const {
    return distanceTo(centre) <= radius;
}

#endif //EVOLUTIONARYNEURALAGENTS_OBSTACLE_H

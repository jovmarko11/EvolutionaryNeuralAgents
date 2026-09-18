//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_TARGET_H
#define EVOLUTIONARYNEURALAGENTS_TARGET_H
#include "../rendering/Drawable.h"
#include "../rendering/DrawParams.h"
#include "../math/Vec2.h"
#include "../configuration/Params.h"

class Target : public Drawable {
    Vec2 position;
    double radius;
    DrawParams drawParams;

public:
    Target(Vec2 position = Vec2(), double radius = 0.5,
           DrawParams drawParams = DrawParams{Params::TargetColor, 0.f});

    Vec2 getPosition() const;

    double getRadius() const;

    // Signed distance: < 0 unutra, > 0 napolju.
    double distanceTo(const Vec2& point) const;

    bool contains(const Vec2& point) const;

    void draw(sf::RenderWindow& window) const override;
};

inline Vec2 Target::getPosition() const { return position; }
inline double Target::getRadius() const { return radius; }

inline bool Target::contains(const Vec2& point) const {
    return distanceTo(point) <= 0.0;
}

#endif //EVOLUTIONARYNEURALAGENTS_TARGET_H

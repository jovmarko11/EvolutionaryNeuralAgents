//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_RAYSENSOR_H
#define EVOLUTIONARYNEURALAGENTS_RAYSENSOR_H
#include "ScanResult.h"
#include "../math/Vec2.h"
#include "../world/World.h"
#include "../configuration/Params.h"

namespace sf { class RenderWindow; }

class RaySensor {
    Vec2 direction;

public:
    explicit RaySensor(Vec2 direction);

    ScanResult scan(const World& world, const Vec2& origin) const;

    void rotate(double turnAngle);

    Vec2 getDirection() const;
    void setDirection(const Vec2& newDirection);

    // normalizedLength je ScanResult::distance: 1.0 znaci da rej nije
    // nista pogodio, manje od 1.0 znaci pogodak na toj relativnoj udaljenosti.
    void draw(sf::RenderWindow& window, const Vec2& origin, double normalizedLength = 1.0) const;
};

inline Vec2 RaySensor::getDirection() const {
    return direction;
}

inline void RaySensor::setDirection(const Vec2& newDirection) {
    direction = newDirection;
}

#endif //EVOLUTIONARYNEURALAGENTS_RAYSENSOR_H

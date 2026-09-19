//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_CIRCLEOBSTACLE_H
#define EVOLUTIONARYNEURALAGENTS_CIRCLEOBSTACLE_H
#include "../Obstacle.h"
#include "../../configuration/Params.h"

class CircleObstacle : public Obstacle {
    double radius;

public:
    CircleObstacle(Vec2 position, double radius,
                    DrawParams drawParams = DrawParams{Params::ObstacleColor, 0.f});

    double getRadius() const;

    double distanceTo(const Vec2& point) const override;

    std::optional<double> intersectRay(const Vec2& origin, const Vec2& direction, double maxDistance) const override;

    void draw(sf::RenderWindow& window) const override;
};

inline double CircleObstacle::getRadius() const { return radius; }

#endif //EVOLUTIONARYNEURALAGENTS_CIRCLEOBSTACLE_H

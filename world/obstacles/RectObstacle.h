//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_RECTOBSTACLE_H
#define EVOLUTIONARYNEURALAGENTS_RECTOBSTACLE_H
#include "../Obstacle.h"
#include "../../configuration/Params.h"

class RectObstacle : public Obstacle {
    double width, height;

public:
    RectObstacle(Vec2 position, double width, double height,
                  DrawParams drawParams = DrawParams{Params::ObstacleColor, 0.f});

    double getWidth() const;
    double getHeight() const;

    double distanceTo(const Vec2& point) const override;

    void draw(sf::RenderWindow& window) const override;
};

inline double RectObstacle::getWidth() const { return width; }
inline double RectObstacle::getHeight() const { return height; }

#endif //EVOLUTIONARYNEURALAGENTS_RECTOBSTACLE_H

//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "CircleObstacle.h"

#include <cmath>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

CircleObstacle::CircleObstacle(Vec2 position, double radius, DrawParams drawParams)
    : Obstacle(position, drawParams), radius(radius) {}

void CircleObstacle::draw(sf::RenderWindow& window) const {
    const auto r = static_cast<float>(radius);

    sf::CircleShape shape(r);
    shape.setOrigin(sf::Vector2f(r, r));
    shape.setPosition(sf::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y)));
    shape.setFillColor(sf::Color(drawParams.color.r, drawParams.color.g, drawParams.color.b, drawParams.color.a));

    window.draw(shape);
}

double CircleObstacle::distanceTo(const Vec2& point) const {
    return (point - position).length() - radius;
}

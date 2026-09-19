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

std::optional<double> CircleObstacle::intersectRay(const Vec2& origin, const Vec2& direction, double maxDistance) const {
    const Vec2 L = position - origin;
    const double tca = L.dot(direction);
    const double d2 = L.lengthSquared() - tca * tca;
    const double r2 = radius * radius;

    if (d2 > r2) return std::nullopt;

    const double thc = std::sqrt(r2 - d2);
    const double t0 = tca - thc;
    const double t1 = tca + thc;
    const double t = (t0 >= 0.0) ? t0 : t1;

    if (t < 0.0 || t > maxDistance) return std::nullopt;
    return t;
}

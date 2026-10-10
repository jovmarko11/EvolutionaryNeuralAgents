//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "RectObstacle.h"

#include <algorithm>
#include <cmath>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

RectObstacle::RectObstacle(Vec2 position, double width, double height, DrawParams drawParams)
    : Obstacle(position, drawParams), width(width), height(height) {}

void RectObstacle::draw(sf::RenderWindow& window) const {
    const auto w = static_cast<float>(width);
    const auto h = static_cast<float>(height);

    sf::RectangleShape shape(sf::Vector2f(w, h));
    shape.setOrigin(sf::Vector2f(w / 2.f, h / 2.f));
    shape.setPosition(sf::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y)));
    shape.setFillColor(sf::Color(drawParams.color.r, drawParams.color.g, drawParams.color.b, drawParams.color.a));

    window.draw(shape);
}

double RectObstacle::distanceTo(const Vec2& point) const {
    // Standardna box SDF: rastojanje do pravougaonika sa centrom u position.
    // d je koliko tacka viri van pravougaonika po svakoj osi (negativno unutra).
    const double dx = std::abs(point.x - position.x) - width / 2.0;
    const double dy = std::abs(point.y - position.y) - height / 2.0;

    // Napolju racuna euklidsko rastojanje do najblize ivice ili ugla,
    // unutra vraca negativno rastojanje do najblize ivice.
    const double outside = Vec2(std::max(dx, 0.0), std::max(dy, 0.0)).length();
    const double inside = std::min(std::max(dx, dy), 0.0);

    return outside + inside;
}

std::optional<double> RectObstacle::intersectRay(const Vec2& origin, const Vec2& direction, double maxDistance) const {
    const double minX = position.x - width / 2.0;
    const double maxX = position.x + width / 2.0;
    const double minY = position.y - height / 2.0;
    const double maxY = position.y + height / 2.0;

    double tmin = 0.0, tmax = maxDistance;

    // X osa
    if (std::abs(direction.x) < 1e-12) {
        if (origin.x < minX || origin.x > maxX) return std::nullopt;
    } else {
        double t1 = (minX - origin.x) / direction.x;
        double t2 = (maxX - origin.x) / direction.x;
        if (t1 > t2) std::swap(t1, t2);
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) return std::nullopt;
    }

    // Y osa — identicno, akumulira se u isti tmin/tmax
    if (std::abs(direction.y) < 1e-12) {
        if (origin.y < minY || origin.y > maxY) return std::nullopt;
    } else {
        double t1 = (minY - origin.y) / direction.y;
        double t2 = (maxY - origin.y) / direction.y;
        if (t1 > t2) std::swap(t1, t2);
        tmin = std::max(tmin, t1);
        tmax = std::min(tmax, t2);
        if (tmin > tmax) return std::nullopt;
    }

    return tmin;
}

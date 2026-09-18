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

//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#include "RaySensor.h"

#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

RaySensor::RaySensor(Vec2 direction) : direction(direction) {}

ScanResult RaySensor::scan(const World& world, const Vec2& origin) const {
    const double hitDistance = world.castRay(origin, direction, Params::sensorMaxDistance);
    const double normalized = hitDistance / Params::sensorMaxDistance;
    const SimStatus status = (hitDistance < Params::sensorMaxDistance) ? SimStatus::Blocked : SimStatus::Ongoing;

    return ScanResult(status, normalized);
}

void RaySensor::rotate(double turnAngle) {
    direction = direction.rotated(turnAngle).normalized();
}

void RaySensor::draw(sf::RenderWindow& window, const Vec2& origin, double normalizedLength) const {
    const double clamped = std::clamp(normalizedLength, 0.0, 1.0);
    const bool hit = clamped < 1.0;

    const Vec2 endpoint = origin + direction * (clamped * Params::sensorMaxDistance);

    const sf::Color lineColor = hit
        ? sf::Color(255, 90, 90, 200)
        : sf::Color(255, 255, 255, 60);

    sf::VertexArray line(sf::PrimitiveType::Lines);
    line.append(sf::Vertex{sf::Vector2f(static_cast<float>(origin.x), static_cast<float>(origin.y)), lineColor});
    line.append(sf::Vertex{sf::Vector2f(static_cast<float>(endpoint.x), static_cast<float>(endpoint.y)), lineColor});

    window.draw(line);
}

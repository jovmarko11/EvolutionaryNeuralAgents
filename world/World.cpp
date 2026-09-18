//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "World.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

World::World(double limitX, double limitY, DrawParams borderParams, DrawParams gridParams)
    : limitX(limitX), limitY(limitY), borderParams(borderParams), gridParams(gridParams) {}


void World::addObstacle(std::unique_ptr<Obstacle> obstacle) { obstacles.push_back(std::move(obstacle)); }
void World::setTarget(Target t) { target = t; }

SimStatus World::inRange(const Vec2& coordinates) const {
    return inRange(coordinates, 0.0);
}

SimStatus World::inRange(const Vec2& centre, double radius) const {
    if (std::abs(centre.x) + radius > limitX || std::abs(centre.y) + radius > limitY) {
        return SimStatus::OutOfBounds;
    }

    for (const auto& obstacle : obstacles) {
        if (obstacle->overlaps(centre, radius)) {
            return SimStatus::Blocked;
        }
    }

    if (target.distanceTo(centre) <= radius) {
        return SimStatus::ReachedTarget;
    }

    return SimStatus::Ongoing;
}

double World::clearance(const Vec2& point) const {
    double nearest = std::min(limitX - std::abs(point.x), limitY - std::abs(point.y));

    for (const auto& obstacle : obstacles) {
        nearest = std::min(nearest, obstacle->distanceTo(point));
    }

    return nearest;
}



void World::draw(sf::RenderWindow& window) const {
    // Kamera (View/Viewport) je posao Renderer-a. World crta iskljucivo
    // u koordinatama sveta i ne zna nista o prozoru.
    drawBorder(window);
    drawGrid(window);
}

void World::drawBorder(sf::RenderWindow& window) const {
    const auto lx = static_cast<float>(limitX);
    const auto ly = static_cast<float>(limitY);

    sf::RectangleShape border(sf::Vector2f(2.f * lx, 2.f * ly));
    border.setOrigin(sf::Vector2f(lx, ly));
    border.setPosition(sf::Vector2f(0.f, 0.f));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(borderParams.color.r, borderParams.color.g, borderParams.color.b, borderParams.color.a));
    border.setOutlineThickness(-borderParams.thickness);

    window.draw(border);
}

void World::drawGrid(sf::RenderWindow& window) const {
    if (gridParams.step <= 0.f) return;

    const auto lx = static_cast<float>(limitX);
    const auto ly = static_cast<float>(limitY);

    sf::Color gridColor(gridParams.color.r, gridParams.color.g, gridParams.color.b,
                         static_cast<std::uint8_t>(static_cast<float>(gridParams.color.a) * gridParams.alphaFactor));

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    for (float x = 0.f; x <= lx; x += gridParams.step) {
        lines.append(sf::Vertex{sf::Vector2f(x, -ly), gridColor});
        lines.append(sf::Vertex{sf::Vector2f(x, ly), gridColor});
        if (x != 0.f) {
            lines.append(sf::Vertex{sf::Vector2f(-x, -ly), gridColor});
            lines.append(sf::Vertex{sf::Vector2f(-x, ly), gridColor});
        }
    }
    for (float y = 0.f; y <= ly; y += gridParams.step) {
        lines.append(sf::Vertex{sf::Vector2f(-lx, y), gridColor});
        lines.append(sf::Vertex{sf::Vector2f(lx, y), gridColor});
        if (y != 0.f) {
            lines.append(sf::Vertex{sf::Vector2f(-lx, -y), gridColor});
            lines.append(sf::Vertex{sf::Vector2f(lx, -y), gridColor});
        }
    }

    window.draw(lines);
}

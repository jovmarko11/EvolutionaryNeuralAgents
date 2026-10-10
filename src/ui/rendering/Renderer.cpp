//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "Renderer.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Viewport.h"
#include "model/world/World.h"
#include "model/simulation/Population.h"

Renderer::Renderer(World& world, Population& population)
    : world(world), population(population) {}


sf::View Renderer::makeWorldView(sf::Vector2u windowSize, const sf::FloatRect& area) const {
    const sf::Vector2f worldSize(2.f * world.getLimitX(), 2.f * world.getLimitY());
    sf::View view({0.f, 0.f}, worldSize);

    // 1) uklopi svet u area (frakcije unutar area)
    const sf::FloatRect f = letterboxViewport(worldSize, area.size);
    // 2) prevedi u frakcije celog prozora (to traži setViewport)
    const sf::Vector2f w(windowSize);
    view.setViewport(sf::FloatRect(
        {(area.position.x + f.position.x * area.size.x) / w.x,
         (area.position.y + f.position.y * area.size.y) / w.y},
        { f.size.x * area.size.x / w.x,
          f.size.y * area.size.y / w.y}));
    return view;
}


void Renderer::render(sf::RenderWindow& window, const sf::FloatRect& area) const {
    window.setView(makeWorldView(window.getSize(), area));
    drawWorld(window);
    drawAgents(window);
}


void Renderer::drawWorld(sf::RenderWindow& window) const {
    world.draw(window);
    world.getTarget().draw(window);

    for (const auto& obstacle : world.getObstacles()) {
        obstacle->draw(window);
    }
}


void Renderer::drawAgents(sf::RenderWindow& window) const {
    for (const auto& agent : population.getAgents()) {
        agent.draw(window);
    }
}

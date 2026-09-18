//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#include "Renderer.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Viewport.h"
#include "../world/World.h"
#include "../simulation/Population.h"

Renderer::Renderer(World& world, Population& population)
    : world(world), population(population) {}


sf::View Renderer::makeWorldView(sf::Vector2u windowSize) const {
    const sf::Vector2f worldSize(
        2.f * static_cast<float>(world.getLimitX()),
        2.f * static_cast<float>(world.getLimitY())
    );

    sf::View view(sf::Vector2f(0.f, 0.f), worldSize);
    view.setViewport(letterboxViewport(worldSize, sf::Vector2f(windowSize)));

    return view;
}


void Renderer::render(sf::RenderWindow& window) const {

    window.setView(makeWorldView(window.getSize()));

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

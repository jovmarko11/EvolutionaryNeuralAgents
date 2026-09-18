//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_RENDERER_H
#define EVOLUTIONARYNEURALAGENTS_RENDERER_H

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf { class RenderWindow; }
class World;
class Population;

/*
 * Renderer je jedini vlasnik kamere.
 *
 * World i Agent crtaju sebe u koordinatama sveta i ne znaju nista o prozoru,
 * zumu ni o odnosu stranica — to su odluke o prikazu i zive ovde.
 */
class Renderer {
private:
    World& world;
    Population& population;

    // Kamera za svet: koji deo sveta se vidi i gde u prozoru se prikazuje.
    sf::View makeWorldView(sf::Vector2u windowSize) const;

    void drawWorld(sf::RenderWindow& window) const;
    void drawAgents(sf::RenderWindow& window) const;

public:
    Renderer(World& world, Population& population);

    void render(sf::RenderWindow& window) const;
};

#endif //EVOLUTIONARYNEURALAGENTS_RENDERER_H

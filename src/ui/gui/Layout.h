//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_LAYOUT_H
#define EVOLUTIONARYNEURALAGENTS_LAYOUT_H
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

// Raspored prozora u pikselima. Cista funkcija velicine prozora, bez ImGui-ja.
struct Layout {
    sf::FloatRect top, bottom, left, right;
    sf::FloatRect sim;   // centralni region za simulaciju
};

Layout computeLayout(sf::Vector2u windowSize);

#endif //EVOLUTIONARYNEURALAGENTS_LAYOUT_H

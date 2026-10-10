//
// Konverzija internog Color struct-a u sf::Color.
//
// Color.h namerno ne zna nista o SFML-u (arhitektonsko pravilo:
// matematika i podaci nezavisni od rendering biblioteke), pa konverzija
// zivi ovde, u rendering sloju.
//

#ifndef EVOLUTIONARYNEURALAGENTS_COLORUTIL_H
#define EVOLUTIONARYNEURALAGENTS_COLORUTIL_H

#include <SFML/Graphics/Color.hpp>

#include "Color.h"

inline sf::Color toSFML(const Color& color) {
    return sf::Color(color.r, color.g, color.b, color.a);
}

#endif //EVOLUTIONARYNEURALAGENTS_COLORUTIL_H

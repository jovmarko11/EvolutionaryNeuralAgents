//
// Created by Marko Jovanovic on 17. 9. 2026..
//

#include "Viewport.h"

sf::FloatRect letterboxViewport(sf::Vector2f worldSize, sf::Vector2f windowSize) {

    if (worldSize.x <= 0.f || worldSize.y <= 0.f ||
        windowSize.x <= 0.f || windowSize.y <= 0.f) {
        return sf::FloatRect({0.f, 0.f}, {1.f, 1.f});
    }

    const float worldAspect = worldSize.x / worldSize.y;
    const float windowAspect = windowSize.x / windowSize.y;

    if (windowAspect > worldAspect) {
        // Prozor je siri od sveta -> suzavamo viewport po x, trake levo i desno.
        const float width = worldAspect / windowAspect;
        return sf::FloatRect({(1.f - width) / 2.f, 0.f}, {width, 1.f});
    }

    // Prozor je visi od sveta -> suzavamo viewport po y, trake gore i dole.
    const float height = windowAspect / worldAspect;
    return sf::FloatRect({0.f, (1.f - height) / 2.f}, {1.f, height});
}

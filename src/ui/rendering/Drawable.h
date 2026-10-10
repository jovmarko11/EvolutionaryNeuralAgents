//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_DRAWABLE_H
#define EVOLUTIONARYNEURALAGENTS_DRAWABLE_H
namespace sf { class RenderWindow; }

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw(sf::RenderWindow& window) const = 0;
};

#endif //EVOLUTIONARYNEURALAGENTS_DRAWABLE_H

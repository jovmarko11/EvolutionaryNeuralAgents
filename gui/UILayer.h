//
// Created by Marko Jovanovic on 19. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_UILAYER_H
#define EVOLUTIONARYNEURALAGENTS_UILAYER_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Layout.h"
#include "UIData.h"
#include "panels/ParametersPanel.h"
#include "panels/PlaceholderPanel.h"
#include "panels/StatisticsPanel.h"
#include "panels/StatusPanel.h"
#include "panels/TransportPanel.h"

class UILayer {
    sf::RenderWindow& window;
    sf::Clock clock;
    Layout layout;
    bool initialized = false;

    TransportPanel transport;
    ParametersPanel parameters;
    StatisticsPanel statistics;
    StatusPanel status;

public:
    explicit UILayer(sf::RenderWindow& window);
    ~UILayer();

    UILayer(const UILayer&) = delete;
    UILayer& operator=(const UILayer&) = delete;

    void init();                                  // poziva se kad je prozor vec otvoren
    void handleEvent(const sf::Event& event);     // prosledi ImGui-ju svaki event
    UIActions beginFrame(const UIData& data);     // Update + crtanje panela, vraca zahteve korisnika
    void render();                                // crta ImGui preko simulacije
    void skipFrame();                             // frejm u kom se ne crta

    const sf::FloatRect& simArea() const { return layout.sim; }
};

#endif //EVOLUTIONARYNEURALAGENTS_UILAYER_H

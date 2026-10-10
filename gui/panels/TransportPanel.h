//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_TRANSPORTPANEL_H
#define EVOLUTIONARYNEURALAGENTS_TRANSPORTPANEL_H
#include "Panel.h"

// Gornja traka: Play/Pause, Step, brzina, generacija, korak epizode, brojaci ishoda.
class TransportPanel : public Panel {
protected:
    void drawContent(const UIData& data, UIActions& actions) override;

public:
    TransportPanel() : Panel("Transport") {
        setBackground({0.11f, 0.15f, 0.24f, 1.f});   // malo svetlija plava traka od ostatka UI-a
        setPadding({14.f, 10.f});
    }
};

#endif //EVOLUTIONARYNEURALAGENTS_TRANSPORTPANEL_H

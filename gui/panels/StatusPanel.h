#ifndef EVOLUTIONARYNEURALAGENTS_STATUSPANEL_H
#define EVOLUTIONARYNEURALAGENTS_STATUSPANEL_H
#include "Panel.h"

// Donja traka: FPS, trajanje generacije, ukupno vreme treninga i precice.
class StatusPanel : public Panel {
protected:
    void drawContent(const UIData& data, UIActions& actions) override;

public:
    StatusPanel() : Panel("Status") {
        setBackground({0.05f, 0.055f, 0.075f, 1.f});
        setPadding({12.f, 5.f});
    }
};

#endif //EVOLUTIONARYNEURALAGENTS_STATUSPANEL_H

//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PLACEHOLDERPANEL_H
#define EVOLUTIONARYNEURALAGENTS_PLACEHOLDERPANEL_H
#include "Panel.h"

// Privremeni prazan panel: samo ispisuje svoje ime. Zamenjuje se pravom klasom
// kad panel dobije sadrzaj.
class PlaceholderPanel : public Panel {
protected:
    void drawContent(const UIData& data, UIActions& actions) override;

public:
    using Panel::Panel;
};

#endif //EVOLUTIONARYNEURALAGENTS_PLACEHOLDERPANEL_H

//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_STATISTICSPANEL_H
#define EVOLUTIONARYNEURALAGENTS_STATISTICSPANEL_H
#include "Panel.h"

// Desni panel: grafici statistike kroz generacije (ImPlot).
class StatisticsPanel : public Panel {
    bool autoFit = true;   // stanje samo ovog panela (ne domena): da li grafik prati nove podatke

protected:
    void drawContent(const UIData& data, UIActions& actions) override;

public:
    StatisticsPanel() : Panel("Statistics") {
        setBackground({0.075f, 0.085f, 0.115f, 1.f});
        setPadding({10.f, 10.f});
    }
};

#endif //EVOLUTIONARYNEURALAGENTS_STATISTICSPANEL_H

#ifndef EVOLUTIONARYNEURALAGENTS_PARAMETERSPANEL_H
#define EVOLUTIONARYNEURALAGENTS_PARAMETERSPANEL_H
#include "Panel.h"

// Levi panel: izmena GA parametara i tezina fitnesa u toku rada.
// Panel drzi lokalni "draft"; program dobija izmene tek na Apply, a trener ih
// primenjuje na granici generacije.
class ParametersPanel : public Panel {
    TrainingParams draft;        // ono sto korisnik trenutno uredjuje
    TrainingParams reference;    // sa cim se draft poredi (isticanje izmenjenih polja)
    bool initialized = false;

    void syncWithProgram(const ParametersInfo& info);
    void drawGaSection(int populationSize);
    void drawFitnessSection();

protected:
    void drawContent(const UIData& data, UIActions& actions) override;

public:
    ParametersPanel() : Panel("Parameters") {
        setBackground({0.075f, 0.085f, 0.115f, 1.f});
        setPadding({10.f, 10.f});
    }
};

#endif //EVOLUTIONARYNEURALAGENTS_PARAMETERSPANEL_H

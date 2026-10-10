//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_UIDATA_H
#define EVOLUTIONARYNEURALAGENTS_UIDATA_H
#include <optional>
#include <span>
#include "learning/statistics/GenerationStatistics.h"
#include "app/config/TrainingParams.h"

// Sve sto GUI sme da CITA iz programa (Program -> UI). Obicne vrednosti, bez
// pokazivaca na domenske klase: paneli ne znaju za Simulation, FixedTimestep...

struct TransportInfo {
    int generation = 0;
    int step = 0, maxSteps = 0;                  // korak unutar generacije
    int alive = 0, reached = 0, crashed = 0, timeout = 0;
    bool paused = false;
    double visibleSpeed = 1.0;
};

struct StatisticsInfo {
    // Pogled (bez kopiranja) na istoriju; vazi samo u okviru ovog frejma.
    std::span<const GenerationStatistics> history;
};

struct ParametersInfo {
    TrainingParams applied;        // vrednosti koje trenutno vaze u treneru
    bool pending = false;          // zahtev je poslat, ceka granicu generacije
    int populationSize = 0;        // gornja granica za tournamentSize
};

struct StatusInfo {
    double lastGenerationSeconds = 0.0;   // trajanje poslednje zavrsene generacije (bez pauza)
    double trainingSeconds = 0.0;         // ukupno vreme treninga (bez pauza)
};

struct UIData {
    TransportInfo transport;
    StatisticsInfo statistics;
    ParametersInfo parameters;
    StatusInfo status;
};

// Sve sto GUI trazi od programa (UI -> Program). Program ih primenjuje.
struct UIActions {
    bool togglePause = false;
    bool step = false;
    std::optional<double> setVisibleSpeed;
    std::optional<TrainingParams> applyTraining;   // Apply dugme; vazi od granice generacije
};

#endif //EVOLUTIONARYNEURALAGENTS_UIDATA_H

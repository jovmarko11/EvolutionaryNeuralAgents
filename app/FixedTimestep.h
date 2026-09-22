//
// Created by Marko Jovanovic on 17. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_FIXEDTIMESTEP_H
#define EVOLUTIONARYNEURALAGENTS_FIXEDTIMESTEP_H

#include <SFML/System/Clock.hpp>

#include "../configuration/Params.h"

class FixedTimestep {
public:
    explicit FixedTimestep(double stepDuration = Params::timeStep, double maxFrameTime = 0.25);

    // Meri proteklo realno vreme i vraca broj koraka za ovaj frejm.
    int beginFrame();

    int beginFrame(double elapsedSeconds);

    void setSpeedMultiplier(double multiplier);
    double getSpeedMultiplier() const;

    bool isPaused() const;
    void togglePause();

    // Jedan korak dok je pauza ukljucena.
    void requestSingleStep();

    double getStepDuration() const;


private:
    sf::Clock clock;

    double stepDuration;    // duzina jednog koraka simulacije, u sekundama
    double maxFrameTime;    // gornja granica za izmereni frejm (spiral of death)
    double speedMultiplier; // 0 = pauza, 1 = realno vreme, 10 = deset puta brze
    double speedBeforePause;

    double accumulator;     // nepotroseno realno vreme
    int pendingSingleSteps;
};

inline double FixedTimestep::getSpeedMultiplier() const { return speedMultiplier; }
inline double FixedTimestep::getStepDuration() const { return stepDuration; }
inline bool FixedTimestep::isPaused() const { return speedMultiplier == 0.0; }

#endif //EVOLUTIONARYNEURALAGENTS_FIXEDTIMESTEP_H

//
// Created by Marko Jovanovic on 17. 9. 2026..
//

#include "FixedTimestep.h"

#include <algorithm>
#include <SFML/System/Time.hpp>

FixedTimestep::FixedTimestep(double stepDuration, double maxFrameTime)
    : stepDuration(stepDuration),
      maxFrameTime(maxFrameTime),
      speedMultiplier(1.0),
      speedBeforePause(1.0),
      accumulator(0.0),
      pendingSingleSteps(0) {}


int FixedTimestep::beginFrame() {
    return beginFrame(static_cast<double>(clock.restart().asSeconds()));
}


int FixedTimestep::beginFrame(double elapsedSeconds) {
    // Ogranicenje na maxFrameTime sprecava "spiral of death": posle dugog
    // zastoja simulacija svesno izgubi deo vremena umesto da trazi stotine
    // koraka u jednom frejmu, cime bi sledeci frejm bio jos duzi.
    const double clamped = std::clamp(elapsedSeconds, 0.0, maxFrameTime);

    accumulator += clamped * speedMultiplier;

    int steps = 0;
    while (accumulator >= stepDuration) {
        accumulator -= stepDuration;
        steps++;
    }

    // Zahtevani pojedinacni koraci idu mimo akumulatora, pa rade i u pauzi.
    steps += pendingSingleSteps;
    pendingSingleSteps = 0;

    return steps;
}


void FixedTimestep::setSpeedMultiplier(double multiplier) {
    speedMultiplier = std::max(0.0, multiplier);

    if (speedMultiplier > 0.0) {
        speedBeforePause = speedMultiplier;
    }
}


void FixedTimestep::togglePause() {
    setSpeedMultiplier(isPaused() ? speedBeforePause : 0.0);
}


void FixedTimestep::requestSingleStep() {
    pendingSingleSteps++;
}

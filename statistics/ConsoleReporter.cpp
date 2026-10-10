//
// Created by Marko Jovanovic on 8. 10. 2026..
//

#include "ConsoleReporter.h"

#include <cstdio>

// Sirine kolona u zaglavlju i u redu moraju ostati usaglasene
// (grupe: gen | fitnes | ishodi | epizode | najbolji agent | GA parametri).
// Ako menjas sirinu u jednom printf-u, promeni je i u ostala dva.

void ConsoleReporter::printHeader() {
    std::puts("     |                  FITNES                  |       ISHODI      |     EPIZODE     |        NAJBOLJI AGENT       |      GA PARAMETRI");
    std::printf("%4s | %8s %7s %7s %7s %7s | %5s %5s %5s | %7s %7s | %7s %5s %6s %6s | %5s %5s %5s %4s\n",
        "gen", "best", "mean", "median", "worst", "stdev",
        "reach", "crash", "tout", "maxProg", "avgStep",
        "prog", "surv", "goal", "crash",
        "elit", "mutR", "mutS", "tour");
    std::puts("-----+------------------------------------------+-------------------+-----------------+-----------------------------+-----------------------");
}

void ConsoleReporter::printRow(const GenerationStatistics& gs) {
    std::printf("%4d | %8.1f %7.1f %7.1f %7.1f %7.1f | %5d %5d %5d | %7.2f %7.1f | %7.1f %5.1f %6.1f %6.1f | %5.3f %5.3f %5.3f %4d\n",
        gs.generation,
        gs.bestFitness, gs.meanFitness, gs.medianFitness, gs.worstFitness, gs.stdevFitness,
        gs.reach, gs.crash, gs.tout,
        gs.maxProgress, gs.averageSteps,
        gs.bestBreakdown.progress, gs.bestBreakdown.survival, gs.bestBreakdown.goal, gs.bestBreakdown.collision,
        gs.gaParams.elitismRatio, gs.gaParams.mutationRatio, gs.gaParams.mutationStrength, gs.gaParams.tournamentSize);
}

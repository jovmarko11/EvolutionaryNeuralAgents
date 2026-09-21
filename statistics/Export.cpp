//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#include "Export.h"

#include <fstream>
#include <iomanip>
#include <limits>
#include <stdexcept>

void Export::exportToCSV(const std::vector<GenerationStatistics>& statistics,
    const std::string& filename) const {

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Export::exportToCSV: ne mogu da otvorim fajl " + filename);
    }

    file << std::setprecision(std::numeric_limits<double>::max_digits10);

    file << "generation,"
            "bestFitness,meanFitness,medianFitness,worstFitness,stdevFitness,"
            "reach,crash,tout,"
            "maxProgress,meanProgress,"
            "averageSteps,minStepsTaken,maxStepsTaken,"
            "bestProgress,bestSurvival,bestGoal,bestCollision,bestSpeed\n";

    for (const GenerationStatistics& gs : statistics) {
        file << gs.generation << ','
             << gs.bestFitness << ',' << gs.meanFitness << ',' << gs.medianFitness << ','
             << gs.worstFitness << ',' << gs.stdevFitness << ','
             << gs.reach << ',' << gs.crash << ',' << gs.tout << ','
             << gs.maxProgress << ',' << gs.meanProgress << ','
             << gs.averageSteps << ',' << gs.minStepsTaken << ',' << gs.maxStepsTaken << ','
             << gs.bestBreakdown.progress << ',' << gs.bestBreakdown.survival << ','
             << gs.bestBreakdown.goal << ',' << gs.bestBreakdown.collision << ','
             << gs.bestBreakdown.speed << '\n';
    }
}

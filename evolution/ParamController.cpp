//
// Created by Marko Jovanovic on 8. 10. 2026..
//

#include "ParamController.h"


void ParamController::updateParameters(const GenerationStatistics& gs) {
    ga.setElitismRatio(updateElitismRatio(gs));
    ga.setMutationRatio(updateMutationRatio(gs));
    ga.setMutationStrength(updateMutationStrength(gs));
    ga.setTournamentSize(updateTournamentSize(gs));
}


double ParamController::updateElitismRatio(const GenerationStatistics& gs) {
    return 0;
}
double ParamController::updateMutationRatio(const GenerationStatistics& gs) {
    return 0;
}
double ParamController::updateMutationStrength(const GenerationStatistics& gs) {
    return 0;
}
int ParamController::updateTournamentSize(const GenerationStatistics& gs) {
    return 1;
}
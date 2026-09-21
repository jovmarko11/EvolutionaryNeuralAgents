//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#include "Statistics.h"

void Statistics::processGeneration(int generation, const std::vector<EpisodeResult>& results) {
    const GenerationStatistics statistics = calculation.calculate(generation, results);

    storage.add(statistics);
}

const GenerationStatistics& Statistics::getLatest() const {
    return storage.getLatest();
}

const std::vector<GenerationStatistics>& Statistics::getHistory() const {
    return storage.getHistory();
}

void Statistics::exportToCSV(const std::string& filename) const {
    exporter.exportToCSV(storage.getHistory(), filename);
}

void Statistics::clear() {
    storage.clear();
}

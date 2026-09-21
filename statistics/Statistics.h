//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_STATISTICS_H
#define EVOLUTIONARYNEURALAGENTS_STATISTICS_H
#include <string>
#include <vector>

#include "Calculation.h"
#include "Export.h"
#include "GenerationStatistics.h"
#include "Storage.h"
#include "../simulation/Episode.h"

class Statistics {
private:
    Calculation calculation;
    Storage storage;
    Export exporter;

public:

    void processGeneration(int generation, const std::vector<EpisodeResult>& results);

    const GenerationStatistics& getLatest() const;

    const std::vector<GenerationStatistics>& getHistory() const;

    void exportToCSV(const std::string& filename) const;

    void clear();

};


#endif //EVOLUTIONARYNEURALAGENTS_STATISTICS_H

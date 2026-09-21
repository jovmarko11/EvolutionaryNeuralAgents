//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_EXPORT_H
#define EVOLUTIONARYNEURALAGENTS_EXPORT_H
#include <string>
#include <vector>
#include "GenerationStatistics.h"


class Export {
public:
    void exportToCSV(const std::vector<GenerationStatistics>& statistics,
        const std::string& filename) const;
};


#endif //EVOLUTIONARYNEURALAGENTS_EXPORT_H
//
// Created by Marko Jovanovic on 12. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_SCANRESULT_H
#define EVOLUTIONARYNEURALAGENTS_SCANRESULT_H

enum class SimStatus;

struct ScanResult {
    SimStatus status;
    double distance;
};

#endif //EVOLUTIONARYNEURALAGENTS_SCANRESULT_H
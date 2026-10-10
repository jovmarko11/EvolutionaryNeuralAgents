//
// Created by Marko Jovanovic on 8. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_CONSOLEREPORTER_H
#define EVOLUTIONARYNEURALAGENTS_CONSOLEREPORTER_H

#include "GenerationStatistics.h"

// Formatira i ispisuje statistiku generacija na standardni izlaz.
// Ne zna nista o Simulation/Statistics/GA - dovoljan mu je jedan zapis
// generacije, koji vec nosi i parametre GA kojima je nastala.
class ConsoleReporter {
public:
    static void printHeader();
    static void printRow(const GenerationStatistics& gs);
};

#endif //EVOLUTIONARYNEURALAGENTS_CONSOLEREPORTER_H

//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_STORAGE_H
#define EVOLUTIONARYNEURALAGENTS_STORAGE_H
#include <vector>

#include "GenerationStatistics.h"


class Storage {
private:
    std::vector<GenerationStatistics> history;

public:
    void add(const GenerationStatistics& statistics);

    const GenerationStatistics& getLatest() const;

    const GenerationStatistics& get(std::size_t generation) const;

    const std::vector<GenerationStatistics>& getHistory() const;

    std::size_t size() const;
    bool empty() const;

    void clear();
};

inline const std::vector<GenerationStatistics>& Storage::getHistory() const { return history; }
inline std::size_t Storage::size() const { return history.size(); }
inline bool Storage::empty() const { return history.empty(); }

#endif //EVOLUTIONARYNEURALAGENTS_STORAGE_H

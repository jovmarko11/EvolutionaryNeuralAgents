//
// Created by Marko Jovanovic on 20. 9. 2026..
//

#include "Storage.h"

#include <cassert>

void Storage::add(const GenerationStatistics& statistics) {
    assert(history.empty() || statistics.generation == history.back().generation + 1);

    history.push_back(statistics);
}

const GenerationStatistics& Storage::getLatest() const {
    assert(!history.empty());
    return history.back();
}

const GenerationStatistics& Storage::get(std::size_t generation) const {
    assert(generation < history.size());
    assert(history[generation].generation == static_cast<int>(generation));

    return history[generation];
}

void Storage::clear() {
    history.clear();
}

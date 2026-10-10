//
// Created by Marko Jovanovic on 11. 9. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_DRAWPARAMS_H
#define EVOLUTIONARYNEURALAGENTS_DRAWPARAMS_H
#include "Color.h"

struct DrawParams {
    Color color = {255, 255, 255, 255};
    float thickness = 0.1f;
    float alphaFactor = 0.25f;
    float step = 1.f;
};

#endif //EVOLUTIONARYNEURALAGENTS_DRAWPARAMS_H

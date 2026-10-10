//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#include "Layout.h"

#include <algorithm>

#include "app/config/Params.h"

Layout computeLayout(sf::Vector2u windowSize) {
    constexpr float leftW = 320, minRightW = 540, topH = 84, bottomH = 32;

    const float W = static_cast<float>(windowSize.x);
    const float H = static_cast<float>(windowSize.y);
    const float midH = H - topH - bottomH;

    // Centar: svet tacne proporcije (40:60) uokviren crnom marginom da UI ne deluje zbijeno.
    // Sav ostatak sirine ide desnom panelu. Ako je prozor uzak, svet se smanjuje
    // a Renderer radi letterbox kao i do sada.
    constexpr float margin = 18.f;
    constexpr float worldAspect = static_cast<float>(Params::worldLimitX / Params::worldLimitY);

    const float simH = std::max(0.f, midH - 2.f * margin);
    const float simW = std::max(0.f, std::min(simH * worldAspect, W - leftW - minRightW - 2.f * margin));
    const float rightW = W - leftW - simW - 2.f * margin;

    Layout layout;
    layout.top    = {{0, 0},             {W, topH}};
    layout.bottom = {{0, H - bottomH},   {W, bottomH}};
    layout.left   = {{0, topH},          {leftW, midH}};
    layout.right  = {{W - rightW, topH}, {rightW, midH}};
    layout.sim    = {{leftW + margin, topH + margin}, {simW, simH}};
    return layout;
}

//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#include "PlaceholderPanel.h"

#include <imgui.h>

void PlaceholderPanel::drawContent(const UIData&, UIActions&) {
    ImGui::TextUnformatted(getName().c_str());
}

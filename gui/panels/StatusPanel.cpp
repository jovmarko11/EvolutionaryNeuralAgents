#include "StatusPanel.h"

#include <cstdio>
#include <imgui.h>

namespace {
    void separator() {
        ImGui::SameLine(0, 14);
        ImGui::TextDisabled("|");
        ImGui::SameLine(0, 14);
    }

    // Sekunde -> hh:mm:ss
    void formatDuration(double seconds, char* out, std::size_t size) {
        const long total = static_cast<long>(seconds);
        std::snprintf(out, size, "%02ld:%02ld:%02ld", total / 3600, (total / 60) % 60, total % 60);
    }
}

void StatusPanel::drawContent(const UIData& data, UIActions&) {
    ImGui::Text("FPS %.0f", static_cast<double>(ImGui::GetIO().Framerate));

    separator();
    if (data.status.lastGenerationSeconds > 0.0)
        ImGui::Text("Last generation %.2f s", data.status.lastGenerationSeconds);
    else
        ImGui::TextDisabled("Last generation -");

    separator();
    char elapsed[16];
    formatDuration(data.status.trainingSeconds, elapsed, sizeof(elapsed));
    ImGui::Text("Training time %s", elapsed);

    separator();
    ImGui::TextDisabled("Space: play/pause    Right: step");
}

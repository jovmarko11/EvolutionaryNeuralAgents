#include "UILayer.h"

#include <stdexcept>
#include <string>
#include <imgui.h>
#include <imgui-SFML.h>
#include <implot.h>

UILayer::UILayer(sf::RenderWindow& window) : window(window) {}

UILayer::~UILayer() {
    if (initialized) {
        ImPlot::DestroyContext();      // pre ImGui-a: ImPlot kontekst zavisi od ImGui konteksta
        ImGui::SFML::Shutdown(window);
    }
}

namespace {
    constexpr float fontSize = 16.f;       // velicina teksta u logickim pikselima prozora
    constexpr float fontOversample = 2.f;  // font se rasterizuje 2x vece i prikazuje umanjeno

    // JetBrains Mono (Medium) iz assets/fonts; ako fajl ne postoji, vazi ugradjeni ImGui font.
    //
    // Zasto oversample: tekstura fonta se iscrtava bilinearno. Ako je glyph rasterizovan u
    // tacno 16 px, a ekran je high-DPI (retina, 2 fizicka piksela po logickom), svaki glyph
    // se rasteguje i izgleda mutno. Rasterizovan u 32 px i umanjen (FontGlobalScale = 0.5)
    // ostaje ostar na retini, a na obicnom ekranu je glatko antialiasovan.
    void loadFont() {
        ImGuiIO& io = ImGui::GetIO();
        const std::string path = std::string(ENA_ASSETS_DIR) + "/fonts/JetBrainsMono-Medium.ttf";

        ImFontConfig config;
        config.OversampleH = 1;
        config.OversampleV = 1;
        config.PixelSnapH = false;

        if (io.Fonts->AddFontFromFileTTF(path.c_str(), fontSize * fontOversample, &config) == nullptr) {
            io.Fonts->AddFontDefault();
        } else {
            io.FontGlobalScale = 1.f / fontOversample;
        }
        if (!ImGui::SFML::UpdateFontTexture())
            throw std::runtime_error("ImGui font texture update failed");
    }

    // Svetliji tekst: ugradjena siva za TextDisabled je na tamnoj pozadini tesko citljiva.
    void applyTextColors() {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]         = {0.93f, 0.95f, 0.98f, 1.f};
        colors[ImGuiCol_TextDisabled] = {0.62f, 0.68f, 0.78f, 1.f};
    }
}

void UILayer::init() {
    if (!ImGui::SFML::Init(window, false))     // false: font ucitavamo sami (loadFont)
        throw std::runtime_error("ImGui-SFML init failed");
    loadFont();
    applyTextColors();
    ImPlot::CreateContext();           // posle ImGui::SFML::Init: treba mu vec postojeci ImGui kontekst
    initialized = true;
}

void UILayer::handleEvent(const sf::Event& event) {
    ImGui::SFML::ProcessEvent(window, event);
}

UIActions UILayer::beginFrame(const UIData& data) {
    ImGui::SFML::Update(window, clock.restart());

    layout = computeLayout(window.getSize());

    UIActions actions;
    transport .draw(layout.top,    data, actions);
    parameters.draw(layout.left,   data, actions);
    statistics.draw(layout.right,  data, actions);
    status    .draw(layout.bottom, data, actions);
    return actions;
}

void UILayer::render() {
    ImGui::SFML::Render(window);
}

void UILayer::skipFrame() {
    ImGui::EndFrame();
}

//
// Created by Marko Jovanovic on 9. 10. 2026..
//

#ifndef EVOLUTIONARYNEURALAGENTS_PANEL_H
#define EVOLUTIONARYNEURALAGENTS_PANEL_H
#include <optional>
#include <string>
#include <utility>

#include <imgui.h>
#include <SFML/Graphics/Rect.hpp>

#include "../UIData.h"

// Osnovna klasa svih panela. draw() radi sve sto je isto za svaki panel
// (fiksni prozor na zadatom pravougaoniku), a izvedena klasa implementira
// samo drawContent(): sta se crta unutar prozora.
class Panel {
private:
    std::string name;
    std::optional<ImVec4> background;   // ako nije zadato, vazi tema
    ImVec2 padding{8.f, 8.f};

protected:
    virtual void drawContent(const UIData& data, UIActions& actions) = 0;

    const std::string& getName() const { return name; }

    // Opcioni izgled panela (poziva se iz konstruktora izvedene klase).
    void setBackground(const ImVec4& color) { background = color; }
    void setPadding(const ImVec2& p) { padding = p; }

public:
    explicit Panel(std::string name) : name(std::move(name)) {}
    virtual ~Panel() = default;

    void draw(const sf::FloatRect& area, const UIData& data, UIActions& actions);
};


#endif //EVOLUTIONARYNEURALAGENTS_PANEL_H

#pragma once
#include "Menu.hpp"
#include "Game.hpp"


class LevelSelection : public Menu {
public:
    LevelSelection();
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate() override;
    void render(sf::RenderWindow& window) override;

private:

};

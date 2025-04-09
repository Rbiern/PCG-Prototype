#pragma once
#include "Menu.hpp"
#include "Game.hpp"


class Level : public Menu {
public:
    Level();
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate() override;
    void render(sf::RenderWindow& window) override;

private:

};

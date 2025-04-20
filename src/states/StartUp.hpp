#pragma once
#include "Menu.hpp"
#include "../core/Game.hpp"


class StartUp : public Menu {
public:
    StartUp();
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RectangleShape startButton;
    sf::Texture background;
    bool flash;
    float flashTimer;
    ResourceManager& rm;
};

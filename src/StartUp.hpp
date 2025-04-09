#pragma once
#include "Menu.hpp"
#include "Game.hpp"



class StartUp : public Menu {
public:
    StartUp();
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate() override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RectangleShape shape;
    sf::Texture texture;
    ResourceManager& rm;
};

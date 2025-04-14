#pragma once
#include "Menu.hpp"
#include "../core/Game.hpp"


class LevelSelection : public Menu {
public:
    LevelSelection();
    ~LevelSelection() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate() override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RectangleShape playButton;
    sf::Texture background;
    sf::Texture exitButton;
    sf::Sprite* exitButtonSprite = nullptr;
    ResourceManager& rm;
};

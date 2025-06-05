#pragma once
#include "Menu.hpp"
#include "../core/Game.hpp"


class LevelSelection : public Menu {
public:
    LevelSelection();
    ~LevelSelection() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Texture background;
    sf::Texture exitButton;
    sf::Texture playButton;
    sf::Sprite* backgroundSprite;
    sf::Sprite* exitButtonSprite;
    sf::Sprite* playButtonSprite;
    ResourceManager& rm;
};
